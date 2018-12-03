# wall-ink-server data verification Nagios script
The main purpose of this code is to check to make sure a web server running `wall-ink-server` is fully functional and giving valid data.

This script mimics the functionality of a `wall-ink` device for use in a monitoring platform such as Nagios.  The script does the same query that a wall-ink device would and, and grabs image data from a `wall-ink-server`, extracts image header data, hashes it, and makes comparisons with given data to see if it is valid. Because it mimics a `wall-ink` display, that means that you must set up a mac address in the `wall-ink-server` device manager database for this script to test. By default, this device entry is created by the setup_sql.sh file in the installation instructions of the wall-ink-server.  If the script runs successfully, and gets valid data from your server, then that means your web server, your device manager database, and you wall-ink-server are set up correctly and thus any functioning `wall-ink` display with a setup mac address should be able to get valid data as well.  You may choose to run this script on a periodic basis with Nagios to monitor your wall-ink-server to ensure that it is running properly, and notify you of any outages.

# Setup and Usage:

### Build the Executable 
Before running `check_wall_ink_server.sh` you first need to build the `wall_ink_data_tool.bin` executable. To do this you must have a C compiler. The included Makefile is setup to compile using GCC, which is standard in most Linux distributions, but you may change it to use a different compiler if you wish.

To build the executable run the command 
```
make
``` 
in this code directory.

After this you should see a `wall_ink_data_tool.bin` in the directory.

Make sure you set the permissions on `wall_ink_data_tool.bin` to executable for whichever user needs to run it, as well as the user who will run the `check_wall_ink_server.sh` script.

### Basic Usage:

While designed for use as a service on a Nagios Server, this script can also be used manually from the command line to check on a server and get debug and verbose output using the `-d` and `-v` options.

To point to your server either use the `-u` option followed by your URL in parentheses like so:
```
./check_wall_ink_server.sh -u "http://your-wall-ink-server.net/get_image.php"
```
or you can change the `url` variable inside the `check_wall_ink_server.sh` file. 

In addition to set the MAC Address you want your Nagios script to test at you can use the `-m` option followed by the MAC Address like so:
```
./check_wall_ink_server.sh -m "AAAABBBBCCCC"
```
or you can change the `mac_address` variable in the script.

Make sure to to change this MAC Address value to the one in the database that you have set up for testing!

For the image key do likewise with the `-i` option followed by your image key:
```
./check_wall_ink_server.sh -i "yourImageKey"
``` 
or you can change the `default_image_key` variable inside the `check_wall_ink_server.sh` file.

This should be the same image key that is used in your wall-ink-server code.

A complete usage example using these options:
```
./check_wall_ink_server.sh -u "http://your-wall-ink-server.net/get_image.php" -m "AAAABBBBCCCC" -i "yourImageKey" 
```

### Script Usage Details:
```
wall-ink check server script

purpose:    Check to make sure a server running `wall-ink-server` is fully 
            functional and giving valid data.

usage: ./check_wall_ink_server.sh                       Run with default settings
   or: ./check_wall_ink_server.sh [options] <arguments> Run with various options

options:
    -v          Set to verbose mode
    -u <url>    Sets the url to check to the url given in it's argument
    -d          Set to debug mode
    -i <key>    Sets the image key value to that given in it's argument
    -m: <mac>   Set the MAC Address to that given in <mac>
    -h          Prints out help

EXIT CODES:
this program gives an exit code according to Nagios service codes
    0 => OK
    1 => WARNING
    2 => CRITICAL
    3 => UNKNOWN
other => CRITICAL: unknown return code
```

### Data Tool Program Usage Details:
```
wall-ink-server data checking program

purpose: tool to verify if data from a wall-ink-server host is valid
         and extract information from the data.

usage: ./wall_ink_data_tool.bin                     Run with default settings
   or: ./wall_ink_data_tool.bin [options] <args>    Run with given options and arguments

The program expects the data to be passed to it via Standard Input (STDIN) 
typically through a shell pipe

options:
    -h:             Display Help and Usage
    -d:             Run in Debug mode
    -b: <bytes>     Set the input buffer/data size to # <bytes>
    -i: <key>       Set the image key to that given in <key>
    -m: <mac>       Set the MAC Address to that given in <mac>
    -v:             Run in Verbose mode
    -C:             Print out the time compressed UNIX timestamp
    -W:             Print out the time to wake UNIX timestamp
    -j:             If used with `-W` or `-C` options, will print the time data in JSON format
```

