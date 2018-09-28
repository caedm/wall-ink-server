#!/usr/local/bin/bash

while getopts "f:m:h" opt; do

case $opt in 

f) FILE=$OPTARG
;;
m) MAC_ADDRESS=$OPTARG
   #	This is where you would include code to query a database
   #	or other resource to look up a schedule for a particular
   #    room or resource based on the mac address of the device.

;;
h) echo "Usage:  example_plugin.sh [OPTION]..."
   echo "Creates a schedule text file containing a day's schedule for a wall-ink device to display"
   echo ""
   echo "   -f 			filename of schedule text file to create"
   echo "   -m			mac address of device to create a schedule file for;"
   echo "                           if no mac address is specified, then a generic"
   echo "                           default example is assumed"
   echo "   -h			display this help and exit"
   echo ""
   echo "Example:   example_plugin.sh -f events.txt -m ABCD1234ABCD"
   echo ""
   exit 0
;;
esac
done

if [ -z "${FILE}" ]; then
	echo "Error:  filename of schedule file must be specified with -f flag"
	exit 1
fi

if [ -z "${MAC_ADDRESS}" ]; then
  #  If a mac address is not specified, print a generic example shcedule text file
  #  simply so something will be displayed on the wall-ink screen and there will not be an error
  #  Do not create a new file here, just append!
   echo "CTB 450 Group Space 2" >> $FILE
   echo "Yacht Club Meeting" >> $FILE
   echo "2018-06-25 09:00:00" >> $FILE
   echo "2018-06-25 13:30:00" >> $FILE
   echo "Test reservation" >> $FILE
   echo "2018-06-25 15:00:00" >> $FILE
   echo "2018-06-25 15:30:00" >> $FILE


else

  #   This is where you would use the data retrieved above from the
  #   database query or other resources to gather data to append to the file.
  echo ""  
  echo "Error:  Script must be modified to handle specific devices."
  echo "         Do not specify a mac address to get generic output"
  echo "         for testing purposes."
  echo ""

fi
        


