#!/bin/bash

#This is a plugin to implement the booked database integration with the wall-ink server

getInfo() {
    mac_address=$1
    mac_address_info=$2
    source ./config/database.sh
    name=`mysql -h $bookedDatabaseServer -u $bookedDatabaseUsername --password=$bookedDatabasePassword -s -N -e "SELECT name FROM collegeresv.resources WHERE resource_id = $resource_id"`
    echo $name >> "$mac_address_info"
    series_ids=`mysql -h $bookedDatabaseServer -u $bookedDatabaseUsername --password=$bookedDatabasePassword -s -N -e "SELECT series_id FROM collegeresv.reservation_resources WHERE resource_id = $resource_id AND series_id NOT IN (SELECT series_id FROM collegeresv.reservation_series WHERE status_id = 2)"`
    series_ids=($series_ids);
    for series_id in "${series_ids[@]}"
    do
        reservation_instance_ids=`mysql -h $bookedDatabaseServer -u $bookedDatabaseUsername --password=$bookedDatabasePassword -s -N -e "SELECT reservation_instance_id FROM collegeresv.reservation_instances WHERE series_id = $series_id AND DATE(CONVERT_TZ(start_date,'+0:00','SYSTEM')) <= DATE(NOW()) AND DATE(CONVERT_TZ(end_date,'+0:00','SYSTEM')) >= DATE(NOW())"`
        reservation_instance_ids=($reservation_instance_ids);
        for reservation_instance_id in "${reservation_instance_ids[@]}"
        do
            title_and_start_date_and_end_date=`mysql -h $bookedDatabaseServer -u $bookedDatabaseUsername --password=$bookedDatabasePassword -s -N -e "SELECT * FROM (SELECT CONVERT_TZ(start_date,'+00:00','SYSTEM'),CONVERT_TZ(end_date,'+00:00','SYSTEM') FROM collegeresv.reservation_instances WHERE reservation_instance_id = $reservation_instance_id) AS A JOIN(SELECT title FROM collegeresv.reservation_series WHERE series_id = $series_id) AS B"`
            echo `mysql -h $bookedDatabaseServer -u $bookedDatabaseUsername --password=$bookedDatabasePassword -s -N -e "SELECT * FROM (SELECT title FROM collegeresv.reservation_series WHERE series_id = $series_id) AS A JOIN (SELECT CONVERT_TZ(start_date,'+00:00','SYSTEM'),CONVERT_TZ(end_date,'+00:00','SYSTEM') FROM collegeresv.reservation_instances WHERE reservation_instance_id = $reservation_instance_id) AS B"`
            start_date=$(echo $title_and_start_date_and_end_date | awk -F $' ' '{print $1" "$2;}')
            end_date=$(echo $title_and_start_date_and_end_date | awk -F $' ' '{print $3" "$4;}')
            series_title=$(echo $title_and_start_date_and_end_date | cut -d' ' -f 5-)
            echo $series_title >> "$mac_address_info"
            echo $start_date >> "$mac_address_info"
            echo $end_date >> "$mac_address_info"
        done
    done
}
