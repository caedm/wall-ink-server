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
#    -m: <mac>   Set the MAC Address to that given in <mac>
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


#temporary filesystem directory, ideally using tmpfs on ramdisk
#since the file is for merely passing data to the C program
#and gets deleted after use
# /dev/shm is the usual place for this on a Linux System
temp_dir="/dev/shm"
content_tmp_file="${temp_dir}/check_server_content.tmp"

#the minimum number of bytes that the returned data must have in order to qualify
min_bytes=1000

#default base url to check
#make sure to change to your server's url or pass your own in the arguments
url="http://your-wall-ink-server.net/get_image.php"

#the default mac address for nagios testing
mac_address="AAAABBBBCCCC"

#values for the url query
test_firmware="nagios"
test_error=0
test_voltage=4.00
test_width=640
test_height=384

#function to generate the query string to append to the url
function generate_query() {
    #print the string back
    printf "?mac_address=%s&firmware=%s&error=%d&voltage=%3.2f&width=%d&height=%d" \
        $mac_address $test_firmware $test_error $test_voltage $test_width $test_height 
}


#the program to use to check the data
check_data_program="./wall_ink_data_tool.bin"

#the default image key
#change this to your own, or pass your's in the arguments
image_key="hunter2"

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
    -m: <mac>   Set the MAC Address to that given in <mac>
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

#string to hold opts to pass to the C program
c_opts=" "

#initialize verbose to be false
verbose=false
#variable flag for if in debug mode
debug_mode=0

#using getopts to parse the arguments
while getopts ":vu:di:hm:" opt; do
    case $opt in
        v)
            #if it gets the -v option for verbose mode
            echo "-v: setting to verbose" >&1
            verbose=true
            #add verbose flag to pass to C code
            c_opts=$c_opts" -v "
            ;;
        u)
            #if it gets the -u <url> option, load the url value from arguments
            echo "-u: setting url to: $OPTARG" >&1
            url=$OPTARG
            ;;
        d)
            #if -d option to turn on debug mode
            echo "-d: debug mode active" >&1
            debug_mode=1
            #add debug flag to pass to C code
            c_opts=$c_opts" -d " 
            ;;
        i)
            #for -i <key> option, setting image key
            echo "-i: setting image key to: $OPTARG" >&1
            #set new value
            image_key=$OPTARG
            ;;
        m)
            #for the -m <mac> option, setting mac address
            echo "-m: setting Mac Address to: $OPTARG" >&1
            #set the new value
            mac_address=$OPTARG
            ;;
        h)
            #if -h help option
            printf " -h: HELP: %s\n" "$help_str" >&1
            exit 0
            ;;
        \?)
            #for invalid/unknown options
            echo "Invalid option: -$OPTARG" >&2
            echo "use option '-h' for help" >&1
            exit 1
            ;;
        :)
            #for options that lack an input value/argument
            echo "Option -$OPTARG requires an argument." >&2
            echo "use option '-h' for help" >&1
            exit 1
            ;;
    esac
done

#only will use colors if set to be verbose
if [ $verbose = true ]; then
    #terminal color codes
    term_red=$(tput setaf 1)
    term_green=$(tput setaf 2)
    term_reset=$(tput sgr0)
    term_yellow=$(tput setaf 3)
else
    #terminal color codes
    #set to blank for not verbose
    term_red=""
    term_green=""
    term_reset=""
    term_yellow=""
fi




#helper function for getting the SHA1 hash of an input variable
function get_bin_hash() {
    #store in local variable hash
    local hash=$( echo -n $1 | sha1sum | awk '{print $1}' )
    #echo the hash value back from the function
    echo -n $hash
}

#get the sha1 hash of the image key
image_key_hash=$( echo -n -e ${image_key} | sha1sum -b | awk '{print $1}' )

#generate the full url
url_query=$url$(generate_query)
#grab the data from the url and put it in the temporary content file
wget --timeout $timeout_sec --tries $num_tries -q $url_query -O $content_tmp_file
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
content=$(cat $content_tmp_file)

#get the http code returned when attempting to connect to the url
http_code=$(curl -s -o /dev/null -w "%{http_code}" $url_query )

#get the number of bytes in the data
#using the flag '-n' echo prevents it adding trailing newline in data
#prevents errors in byte count
bytes=$(echo -n $content| wc -c ) 

#verbose output
if [ $verbose = true ]; then
    #print the query
    echo "URL Query: ${url_query}"
    #to get slightly more verbose http info for debugging
    http_info=$(wget --server-response -nv $url_query -O /dev/null 2>&1 | grep -F HTTP  )
    #echo the http info
    echo "http_info = $http_info"
    #echo out the http_code for debugging
    echo "http_code = $http_code"
    #echo out the bytes for debugging
    echo "bytes = $bytes"
fi


#check if the url call worked and got HTTP code 200 for OK
if [ $http_code -eq "200" ]; then
    #check if amount of data is good
    if [ $bytes -gt $min_bytes ]; then
        #OK - data meets minimum byte requirement

        #call the c program
        cat $content_tmp_file | $check_data_program \
                                -b $bytes $c_opts -i $image_key \
                                -m $mac_address
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
        #based on the return value, give proper output and exit code
        case $c_exit_code in
            0)
                echo "${term_green}SUCCESS - data is valid${term_reset}" >&1
                exit 0
                ;;
            1)
                echo "${term_yellow}WARNING - error occured in data checking program${term_reset}" >&1
                exit 1
                ;;
            2)
                echo "${term_red}CRITICAL - data is not valid!${term_reset}" >&1
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



