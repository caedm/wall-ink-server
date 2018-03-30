 #!/bin/bash

server=`grep server ../device_manager/dbconfig.php | grep -o "[[:alnum:].]*" | grep -v server`
username=`grep username ../device_manager/dbconfig.php | grep -o "[[:alnum:]\-]*" | grep -v username`
password=`grep password ../device_manager/dbconfig.php | grep -o "[[:alnum:].]*" | grep -v password`
if [[ ! -e "/auto/grp3/door-display/www/voltage_monitor/data" ]]; then
    mkdir /auto/grp3/door-display/www/voltage_monitor/data
fi
ESP_DEVICES=`mysql -h $server -u $username --password=$password -s -N -e 'SELECT device_id FROM \`door-display\`.devices'`
ESP_DEVICES_ARRAY=($ESP_DEVICES);
for device_id in "${ESP_DEVICES_ARRAY[@]}"
do
    voltage=`mysql -h $server -u $username --password=$password -s -N -e 'SELECT voltage FROM \`door-display\`.devices WHERE device_id = '$device_id`
    mac_address=`mysql -h $server -u $username --password=$password -s -N -e 'SELECT mac_address FROM \`door-display\`.devices WHERE device_id = '$device_id`
    if [[ ! -f "/auto/grp3/door-display/www/voltage_monitor/data/$mac_address.rrd" ]]; then
        rrdtool create /auto/grp3/door-display/www/voltage_monitor/data/$mac_address.rrd \
            --start `date +%s` \
            --step 1800 \
            DS:voltage:GAUGE:3600:1:4 \
            RRA:AVERAGE:0.5:48:365 \
            RRA:AVERAGE:0.5:1:334
    fi
    rrdtool update /auto/grp3/door-display/www/voltage_monitor/data/$mac_address.rrd N:$voltage
done
