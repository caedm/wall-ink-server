source ./database.sh
DATE=`date +%Y-%m-%d`
ESP_DEVICES=`mysql -h $server -u $username --password=$password -s -N -e 'SELECT resource_id FROM \`door-display\`.devices'`
ESP_DEVICES_ARRAY=($ESP_DEVICES);
for resource_id in "${ESP_DEVICES_ARRAY[@]}"
do
    mac_address=`mysql -h $server -u $username --password=$password -s -N -e 'SELECT mac_address FROM \`door-display\`.devices WHERE resource_id = '$resource_id`
    device_type=`mysql -h $server -u $username --password=$password -s -N -e 'SELECT device_type FROM \`door-display\`.devices WHERE resource_id = '$resource_id`
    name=`mysql -h $server -u $username --password=$password -s -N -e "SELECT name FROM collegeresv.resources WHERE resource_id = $resource_id"`
    series_ids=`mysql -h $server -u $username --password=$password -s -N -e "SELECT series_id FROM collegeresv.reservation_resources WHERE resource_id = $resource_id"`
    series_ids=($series_ids);
    echo "Room Name: " $name
    echo "Resource ID: " $resource_id
    echo "Device Type: " $device_type
    echo "MAC Address: " $mac_address
    echo $mac_address > fromDB
    echo $name >> fromDB
    echo $DATE >> fromDB
    echo $device_type >> fromDB
    for series_id in "${series_ids[@]}"
    do
        echo "Series ID: " $series_id
        reservation_instance_ids=`mysql -h $server -u $username --password=$password -s -N -e "SELECT reservation_instance_id FROM collegeresv.reservation_instances WHERE series_id = $series_id AND DATE(start_date) <= DATE(NOW()) AND DATE(end_date) >= DATE(NOW())"`
        reservation_instance_ids=($reservation_instance_ids);
        for reservation_instance_id in "${reservation_instance_ids[@]}"
        do
            series_title=`mysql -h $server -u $username --password=$password -s -N -e "SELECT title FROM collegeresv.reservation_series WHERE series_id = $series_id"`
            start_date=`mysql -h $server -u $username --password=$password -s -N -e "SELECT start_date FROM collegeresv.reservation_instances WHERE reservation_instance_id = $reservation_instance_id"`
            end_date=`mysql -h $server -u $username --password=$password -s -N -e "SELECT end_date FROM collegeresv.reservation_instances WHERE reservation_instance_id = $reservation_instance_id"`
            echo "Reservation Instance ID: " $reservation_instance_id
            echo "Reservation Series Title: " $series_title
            echo "Start: " $start_date
            echo "End: " $end_date
            echo $series_title >> fromDB
            echo $start_date >> fromDB
            echo $end_date >> fromDB
        done
    done
    ./genimg
    mac_address_base_64="../www/"
    mac_address_base_64+=$mac_address
    mac_address_base_64+=".base64"
    openssl base64 -in $mac_address -out $mac_address_base_64 -A
    rm $mac_address
done
