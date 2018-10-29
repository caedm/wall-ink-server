#!/bin/bash

#wall-ink check server script
#
#purpose:   Check to make sure a server running `wall-ink-server` is 
#           fully functional and giving valid data.
#
#usage: ./check_wall_ink_server.sh                       Run with default settings
#   or: ./check_wall_ink_server.sh [options] <arguments> Run with various options
#
#options:
#    -v          Set to verbose mode
#    -u <url>    Sets the url to check to the url given in it's argument
#    -d          Set to debug mode
#    -i <key>    Sets the image key value to that given in it's argument
#    -h          Prints out help
#
#EXIT CODES:
#this program gives an exit code according to Nagios service codes
#    0 => OK
#    1 => WARNING
#    2 => CRITICAL
#    3 => UNKNOWN
#other => CRITICAL: unknown return code
#

#       DETAILED PROGRAM DESCRIPTION

#this contains a script `check_wall_ink_server.sh` which grabs data from a 
#`wall-ink-server` url and verifies if it is valid data by passing the data 
#to the `check_wall_ink_data.bin` executable. `check_wall_ink_data.bin` 
#recieves the data via standard input (stdin), it then takes the first 20 bytes 
#and compares it to a hash of the combined data of two hashes, the hash of 
#the image key together with the hash of the time data (the next 8 bytes). 
#If the data is valid, and the image key is correct, this final hash should 
#match the first 20 bytes of the data. `check_wall_ink_data.bin` returns a code 
#back to the `check_wall_ink_server.sh` script which calls it reporting on 
#success or failure of the data check. `check_wall_ink_server.sh` then uses 
#this response to give a proper exit code or error when it exits, this code 
#is sent back to the nagios server which calls it. 
#
#If when `check_wall_ink_server.sh` is run and there is an error in connecting 
#to the server, or it get's an http status code other than `200` (ok), then it 
#will exit with a critical exit code before calling the 
#`check_wall_ink_data.bin`. It also does this if the data size is too small to 
#be valid.
#


#temporary filesystem directory, ideally using tmpfs on ramdisk
#since the file is for merely passing data to the C program
#and gets deleted after use
# /dev/shm is the usual place for this on a Linux System
temp_dir="/dev/shm"
content_tmp_file="$temp_dir/check_server_content.tmp"

#the minimum number of bytes that the returned data must have in order to qualify
min_bytes=1000

#default url to check
#make sure to change to your server's url or pass your own in the arguments
url="http://your-wall-ink-server.net/get_image.php?mac_address=AAAABBBBCCCC&firmware=nagios&error=0&voltage=4.00"

#the program to use to check the data
check_data_program="./check_wall_ink_data.bin"

#the default image key without NULL
#change this to your own, or pass your's in the arguments
default_image_key="hunter2"

#storing the image key with ending NULL for use in getting it's hash 
#to match those done in current wall-ink-server code
image_key_w_NULL=$default_image_key"\0"

#how many times wget should try to connect to the server
num_tries=1
#how long in seconds should wget wait before a timeout on the server
timeout_sec=10

#variable to store our program's name
prog_name=$0

#string with the options and their usage
options_str="options:
    -v          Set to verbose mode
    -u <url>    Sets the url to check to the url given in it's argument
    -d          Set to debug mode
    -i <key>    Sets the image key value to that given in it's argument
    -h          Prints out help"

#string with the help dialog
help_str="
wall-ink check server script

purpose:    Check to make sure a server running wall-ink-server is fully 
            functional and giving valid data.

usage: $prog_name                       Run with default settings
   or: $prog_name [options] <arguments> Run with various options

$options_str
"

#terminal color codes
term_red=$(tput setaf 1)
term_green=$(tput setaf 2)
term_reset=$(tput sgr0)
term_yellow=$(tput setaf 3)

#string to hold opts to pass to the C program
c_opts=" "

#initialize verbose to be false
verbose=false
#variable flag for if in debug mode
debug_mode=0

#using getopts to parse the arguments
while getopts ":vu:di:h" opt; do
    case $opt in
        v)
            #if it gets the -v option for verbose mode
            echo "-v: setting to verbose" >&2
            verbose=true
            #add verbose flag to pass to C code
            c_opts=$c_opts" -v "
            ;;
        u)
            #if it gets the -u <url> option, load the url value from arguments
            echo "-u: setting url to: $OPTARG" >&2
            url=$OPTARG
            ;;
        d)
            #if -d option to turn on debug mode
            echo "-d: debug mode active" >&2
            debug_mode=1
            #add debug flag to pass to C code
            c_opts=$c_opts" -d " 
            ;;
        i)
            #for -i <key> option, setting image key
            echo "-i: setting image key to: $OPTARG" >&2
            #set new value
            default_image_key=$OPTARG
            #add NULL to end 
            image_key_w_NULL=$default_image_key"\0"
            ;;
        h)
            #if -h help option
            printf " -h: HELP: %s\n" "$help_str" >&2
            exit 0
            ;;
        \?)
            #for invalid/unknown options
            echo "Invalid option: -$OPTARG" >&2
            echo "use option '-h' for help" >&2
            exit 1
            ;;
        :)
            #for options that lack an input value/argument
            echo "Option -$OPTARG requires an argument." >&2
            echo "use option '-h' for help" >&2
            exit 1
            ;;
    esac
done


#helper function for getting the SHA1 hash of an input variable
function get_bin_hash() {
    #store in local variable hash
    local hash=$( echo -n $1 | sha1sum | awk '{print $1}' )
    #echo the hash value back from the function
    echo -n $hash
}

#get the sha1 hash of the image key
#get the hash, using -e option to include NULL '\0' in string
#this is so it matches what wall-ink-server is currently doing in it's code
image_key_hash=$( echo -n -e ${image_key_w_NULL} | sha1sum -b | awk '{print $1}' )


#grab the data from the url and put it in the temporary content file
wget --timeout $timeout_sec --tries $num_tries -q $url -O $content_tmp_file
#get the exit code of wget
wget_ret=$?

#check if it got an error, other than a server error response (exit 8)
#usually from a timeout
if [ $wget_ret -ne 0 ] && [ $wget_ret -ne 8 ]; then
    #print output
    echo "${term_red}CRITICAL - Network or other failure!${term_reset}"
    #exit with critical error
    exit 2
fi

#get the data from the temporary content file and dump it in a variable
#for the script to use where needed, mainly for debugging
content=$(cat $content_tmp_file);

#get the http code returned when attempting to connect to the url
http_code=$(curl -s -o /dev/null -w "%{http_code}" $url )

#get the number of bytes in the data
#using the flag '-n' echo prevents it adding trailing newline in data
#prevents errors in byte count
bytes=$(echo -n $content| wc -c ) 

#verbose output
if [ $verbose = true ]; then
    #to get slightly more verbose http info for debugging
    http_info=$(wget --server-response -nv $url -O /dev/null 2>&1 | grep -F HTTP  )
    echo "http_info = $http_info"
    #echo out the bytes for debugging
    echo "bytes = $bytes"
fi


#debug output
if [ $debug_mode -eq 1 ]; then
    
    #echo out the content data for debugging
    echo "content = "
    echo $content
    
    #output with printf to see any difference (tabs/newline vs. spaces)
    echo "printf on content"
    printf "%s\n" $content
    
    #echo out the http_code for debugging
    echo "http_code = $http_code"

    #print the image key hash for debug
    echo "image_key_hash = $image_key_hash"
    #hash stuff
    
    #get first 20 bytes of data
    head_data=$(echo -n $content | head -c 20)
    echo "head_data = "
    echo $head_data
    
    #head_data_hex=$(echo -n $head_data | xxd -p)
    head_data_hex=$(echo -n $head_data | xxd -p)
    echo "head_data_hex = $head_data_hex"
    
    #get next 8 bytes of data, the time data
    time_data=$( echo -n $content | tail -c+21 | head -c 8 )
    echo "time_data = "
    echo $time_data
   
    #get hex of the time data for debug 
    time_data_hex=$(echo -n $time_data | xxd -p)
    echo "time_data_hex = $time_data_hex"
    
    #generate hash of time_data
    time_hash=$( get_bin_hash $time_data );
    echo "time_hash = $time_hash"
    
fi

#check if the url call worked and got HTTP code 200 for OK
if [ $http_code -eq "200" ]; then
    #check if amount of data is good
    if [ $bytes -gt $min_bytes ]; then
        #OK - data meets minimum byte requirement

        #call the c program
        cat $content_tmp_file | $check_data_program \
                                -b $bytes $c_opts -i $default_image_key 
        #store the exit code of the program 
        #(system variable $? stores the last return value)
        c_exit_code=$?
        #remove the temporary file
        rm $content_tmp_file
        
        if [ $verbose = true ] || [ $debug_mode -eq 1 ]; then
            #print the exit code of the program
            echo "exit code of $check_data_program is: $c_exit_code"
        fi

        #if the C code exited with an error
        case $c_exit_code in
            0)
                echo "${term_green}SUCCESS - data is valid${term_reset}" >&2
                exit 0
                ;;
            1)
                echo "${term_yellow}WARNING - error occured in data checking program${term_reset}" >&2
                exit 1
                ;;
            2)
                echo "${term_red}CRITICAL - data is not valid!${term_reset}" >&2
                exit 2
                ;;
        esac
	    exit 0
    else
        #if the data was too small
        echo "${term_red}CRITICAL - data is too small or NULL${term_reset}"
        #remove the temporary file
        rm $content_tmp_file
	    exit 2
    fi
else
    #if there is an HTTP/url error, then exit critical
    echo "${term_red}CRITICAL - error with url or connection, HTTP code:${term_reset} $http_code"
    #remove the temporary file
    rm $content_tmp_file
    exit 2
fi

#if this code somehow gets to here (it shouldn't), then there is a problem,
#so it will be an unknown other code
exit 4



