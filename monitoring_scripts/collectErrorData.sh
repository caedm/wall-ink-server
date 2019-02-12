 #!/bin/bash

source $1
if [[ ! -e "$runTimeWebDirectory/log" ]]; then
    mkdir -p $runTimeWebDirectory/log
fi
ESP_DEVICES=`mysql -h $deviceDatabaseServer -u $deviceDatabaseUsername --password=$deviceDatabasePassword --database=$deviceDatabaseName -s -N -e "SELECT mac_address FROM devices where last_checked_in > CURRENT_DATE - INTERVAL 1 DAY"`
ESP_DEVICES_ARRAY=($ESP_DEVICES);
TODAY=`date +%Y-%m-%d`
YESTERDAY=`date -d "-1 days" +%Y-%m-%d`
echo "" > $runTimeWebDirectory/log/errors.log
for mac_address in "${ESP_DEVICES_ARRAY[@]}"
do
    NUM_ERRORS=`grep "$TODAY\|$YESTERDAY" $runTimeWebDirectory/log/$mac_address.log | grep -cv "Error Code: 0"`
    echo $mac_address: $NUM_ERRORS
    echo $mac_address $NUM_ERRORS >> $runTimeWebDirectory/log/errors.log
done

chmod 660 $runTimeWebDirectory/log/errors.log
