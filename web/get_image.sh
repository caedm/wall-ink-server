#!/bin/bash
server=`grep server dbconfig.php | grep -o "[[:alnum:].]*" | grep -v server`
username=`grep username dbconfig.php | grep -o "[[:alnum:]\-]*" | grep -v username`
password=`grep password dbconfig.php | grep -o "[[:alnum:].]*" | grep -v password`
DATE=`date +%Y-%m-%d`
TIME=`date +%H:%M`
mac_address=$1
voltage=$2
resource_id_and_device_type_and_orientation_and_old_voltage=`mysql -h $server -u $username --password=$password -s -N -e 'SELECT resource_id,device_type,orientation,voltage FROM \`door-display\`.devices WHERE mac_address = "'$mac_address\"`
resource_id=$(echo $resource_id_and_device_type_and_orientation_and_old_voltage | awk '{print $1;}')
device_type=$(echo $resource_id_and_device_type_and_orientation_and_old_voltage | awk '{print $2;}')
orientation=$(echo $resource_id_and_device_type_and_orientation_and_old_voltage | awk '{print $3;}')
old_voltage=$(echo $resource_id_and_device_type_and_orientation_and_old_voltage | awk '{print $4;}')
volt_comp() {
    awk -v n1="$1" -v n2="$2" 'BEGIN {if (n1+0.25<n2+0) exit 0; exit 1}'
}
if volt_comp "$old_voltage" "$voltage"; then
    `mysql -h $server -u $username --password=$password -s -N -e 'UPDATE \`door-display\`.devices SET batteries_replaced_date = NOW() WHERE mac_address = "'$mac_address\"`
fi
`mysql -h $server -u $username --password=$password -s -N -e 'UPDATE \`door-display\`.devices SET voltage = '$voltage' WHERE mac_address = "'$mac_address\"`
name=`mysql -h $server -u $username --password=$password -s -N -e "SELECT name FROM collegeresv.resources WHERE resource_id = $resource_id"`
series_ids=`mysql -h $server -u $username --password=$password -s -N -e "SELECT series_id FROM collegeresv.reservation_resources WHERE resource_id = $resource_id AND series_id NOT IN (SELECT series_id FROM collegeresv.reservation_series WHERE status_id = 2)"`
series_ids=($series_ids);
echo "Room Name: $name"
echo "Resource ID: " $resource_id
echo "Device Type: " $device_type
echo "MAC Address: " $mac_address
mac_address_info="image_data/"
mac_address_info+=$mac_address
mac_address_info+=".info"
echo $mac_address > "$mac_address_info"
echo $name >> "$mac_address_info"
echo $DATE >> "$mac_address_info"
echo $TIME >> "$mac_address_info"
echo $device_type >> "$mac_address_info"
echo $voltage >> "$mac_address_info"
echo $orientation >> "$mac_address_info"
for series_id in "${series_ids[@]}"
do
    #echo "Series ID: " $series_id
    reservation_instance_ids=`mysql -h $server -u $username --password=$password -s -N -e "SELECT reservation_instance_id FROM collegeresv.reservation_instances WHERE series_id = $series_id AND DATE(CONVERT_TZ(start_date,'+00:00','-07:00')) <= DATE(NOW()) AND DATE(CONVERT_TZ(end_date,'+00:00','-07:00')) >= DATE(NOW())"`
    reservation_instance_ids=($reservation_instance_ids);
    for reservation_instance_id in "${reservation_instance_ids[@]}"
    do
        title_and_start_date_and_end_date=`mysql -h $server -u $username --password=$password -s -N -e "SELECT * FROM (SELECT CONVERT_TZ(start_date,'+00:00','-07:00'),CONVERT_TZ(end_date,'+00:00','-07:00') FROM collegeresv.reservation_instances WHERE reservation_instance_id = $reservation_instance_id) AS A JOIN(SELECT title FROM collegeresv.reservation_series WHERE series_id = $series_id) AS B"`
        echo `mysql -h $server -u $username --password=$password -s -N -e "SELECT * FROM (SELECT title FROM collegeresv.reservation_series WHERE series_id = $series_id) AS A JOIN (SELECT CONVERT_TZ(start_date,'+00:00','-07:00'),CONVERT_TZ(end_date,'+00:00','-07:00') FROM collegeresv.reservation_instances WHERE reservation_instance_id = $reservation_instance_id) AS B"`
        start_date=$(echo $title_and_start_date_and_end_date | awk -F $' ' '{print $1" "$2;}')
        end_date=$(echo $title_and_start_date_and_end_date | awk -F $' ' '{print $3" "$4;}')
        series_title=$(echo $title_and_start_date_and_end_date | cut -d' ' -f 5-)
        echo $series_title >> "$mac_address_info"
        echo $start_date >> "$mac_address_info"
        echo $end_date >> "$mac_address_info"
    done
done
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
echo $mac_address >> log.txt
echo $voltage >> log.txt
echo $DATE >> log.txt
echo $TIME >> log.txt

