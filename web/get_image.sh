#!/bin/bash

server=`grep server device_manager/dbconfig.php | grep -o "[[:alnum:].]*" | grep -v server`
username=`grep username device_manager/dbconfig.php | grep -o "[[:alnum:]\-]*" | grep -v username`
password=`grep password device_manager/dbconfig.php | grep -o "[[:alnum:].]*" | grep -v password`
DATE=`date -d "+2 minutes" +%Y-%m-%d`
TIME=`date -d "+2 minutes" +%H:%M`
mac_address=$1
voltage=$2
errorCode=$3
resource_id_and_device_type_and_orientation_and_old_voltage=`mysql -h $server -u $username --password=$password -s -N -e 'SELECT resource_id,device_type,orientation,voltage FROM \`door-display\`.devices WHERE mac_address = "'$mac_address\"`
[[ -z "$resource_id_and_device_type_and_orientation_and_old_voltage" ]] && exit 1
resource_id=$(echo $resource_id_and_device_type_and_orientation_and_old_voltage | awk '{print $1;}')

#if there is a 4th parameter, that means this is being called from get_png.php. Otherwise, it's probably a display being updated.
if [ $4 -eq $7 ]
then
    device_type=$(echo $resource_id_and_device_type_and_orientation_and_old_voltage | awk '{print $2;}')
    orientation=$(echo $resource_id_and_device_type_and_orientation_and_old_voltage | awk '{print $3;}')
    `mysql -h $server -u $username --password=$password -s -N -e 'UPDATE \`door-display\`.devices SET voltage = '$voltage', last_checked_in = NOW() WHERE mac_address = "'$mac_address\"`
else
    device_type=$4
    orientation="0"
fi
old_voltage=$(echo $resource_id_and_device_type_and_orientation_and_old_voltage | awk '{print $4;}')
volt_comp() {
    awk -v n1="$1" -v n2="$2" 'BEGIN {if (n1+0.35<n2+0) exit 0; exit 1}'
}
if volt_comp "$old_voltage" "$voltage"; then
    `mysql -h $server -u $username --password=$password -s -N -e 'UPDATE \`door-display\`.devices SET batteries_replaced_date = NOW() WHERE mac_address = "'$mac_address\"`
fi
echo "Resource ID: " $resource_id
echo "Device Type: " $device_type
echo "MAC Address: " $mac_address
mac_address_info="image_data/"
mac_address_info+=$mac_address
mac_address_info+=".info"
echo $mac_address > "$mac_address_info"
echo $DATE >> "$mac_address_info"
echo $TIME >> "$mac_address_info"
echo $device_type >> "$mac_address_info"
echo $voltage >> "$mac_address_info"
echo $orientation >> "$mac_address_info"
echo $resource_id >> "$mac_address_info"
source ./booked.sh
getInfo $mac_address $mac_address_info
echo `./genimg $mac_address_info`
web_dir="../www/"
mac_address_compressed=$mac_address
mac_address_compressed+=".compressed"
mac_address_compressed_web_dir=$web_dir
mac_address_compressed_web_dir+=$mac_address_compressed
#    read -p "Press [Enter] key to continue..."

maximum_size=100000
actual_size=$(wc -c <"log.txt")
if [[ $actualsize -ge $maximum_size ]]; then
    mv log.txt log.txt.prev
fi
echo "\nMAC Address:" >> log.txt
echo $mac_address >> log.txt
echo "Voltage:" >> log.txt
echo $voltage >> log.txt
echo "Date:" >> log.txt
echo $DATE >> log.txt
echo "Time:" >> log.txt
echo $TIME >> log.txt
echo "ErrorCode:" >> log.txt
echo $errorCode >> log.txt

