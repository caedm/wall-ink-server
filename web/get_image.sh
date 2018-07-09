#!/bin/bash

source ./config/database.sh
DATE=`date -d "+2 minutes" +%Y-%m-%d`
TIME=`date -d "+2 minutes" +%H:%M`
mac_address=$1
voltage=$2
errorCode=$3
resource_id_and_device_type_and_orientation_and_old_voltage_and_scheduling_system=`mysql -h $deviceDatabaseServer -u $deviceDatabaseUsername --password=$deviceDatabasePassword -s -N -e 'SELECT resource_id,device_type,orientation,voltage,scheduling_system FROM \`door-display\`.devices WHERE mac_address = "'$mac_address\"`
[[ -z "$resource_id_and_device_type_and_orientation_and_old_voltage_and_scheduling_system" ]] && exit 1
resource_id=$(echo $resource_id_and_device_type_and_orientation_and_old_voltage_and_scheduling_system | awk '{print $1;}')

#if there is a 4th parameter, that means this is being called from get_png.php. Otherwise, it's probably a display being updated.
if [ $4 -eq $7 ]
then
    device_type=$(echo $resource_id_and_device_type_and_orientation_and_old_voltage_and_scheduling_system | awk '{print $2;}')
    orientation=$(echo $resource_id_and_device_type_and_orientation_and_old_voltage_and_scheduling_system | awk '{print $3;}')
    `mysql -h $deviceDatabaseServer -u $deviceDatabaseUsername --password=$deviceDatabasePassword -s -N -e 'UPDATE \`door-display\`.devices SET voltage = '$voltage', last_checked_in = NOW() WHERE mac_address = "'$mac_address\"`
else
    device_type=$4
    orientation="0"
fi
old_voltage=$(echo $resource_id_and_device_type_and_orientation_and_old_voltage_and_scheduling_system | awk '{print $4;}')
scheduling_system=$(echo $resource_id_and_device_type_and_orientation_and_old_voltage_and_scheduling_system | awk '{print $5;}')
volt_comp() {
    awk -v n1="$1" -v n2="$2" 'BEGIN {if (n1+0.35<n2+0) exit 0; exit 1}'
}
if volt_comp "$old_voltage" "$voltage"; then
    `mysql -h $deviceDatabaseServer -u $deviceDatabaseUsername --password=$deviceDatabasePassword -s -N -e 'UPDATE \`door-display\`.devices SET batteries_replaced_date = NOW() WHERE mac_address = "'$mac_address\"`
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

if [ $scheduling_system == 0 ]
then
    #Booked Integration
    source ./booked.sh
    getInfo $mac_address $mac_address_info
elif [ $scheduling_system == 1 ]
then
    #Google Calendar Integration
    cd google
    plugin=`./gcal -cal "$resource_id@group.calendar.google.com" -sec="./client_secret.json"`
    cd ..
    printf "$plugin" >> $mac_address_info
fi

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
echo "MAC Address: $mac_address" >> log.txt
echo "Voltage: $voltage" >> log.txt
echo "$DATE $TIME" >> log.txt
echo "ErrorCode: $errorCode" >> log.txt
echo "" >> log.txt
cd google
echo `pwd` >> log.txt
