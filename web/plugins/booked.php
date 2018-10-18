<?php
require("$_SERVER[DOCUMENT_ROOT]/plugin_dependencies/iPlugin.php");
require("$_SERVER[DOCUMENT_ROOT]/config/dbconfig.php");
if ($config->bookedIntegrationActive == "true") {
    class bookedPlugin implements iPlugin {
        public function getIndex() {
            return 0;
        } 
        public function getName() {
            return "Booked";
        } 
        public function isActive($config) {
            return $config->bookedIntegrationActive;
        }
        public function getResources($config) {
            $rooms = array();
            $mysqli = mysqli_connect($config->bookedDatabaseServer, $config->bookedDatabaseUsername, $config->bookedDatabasePassword, $config->bookedDatabaseName);
            $resources = mysqli_query($mysqli, "SELECT resource_id,name FROM resources");
            while($room = $resources->fetch_assoc()){
                $rooms[ $room["resource_id"] ] = $room["name"];
            }
            return $rooms;
        }
        public function getSchedule($config, $resourceId) {
            $mysqli = mysqli_connect($config->bookedDatabaseServer, $config->bookedDatabaseUsername, $config->bookedDatabasePassword, $config->bookedDatabaseName);
            $name = mysqli_query($mysqli, "SELECT name FROM resources WHERE resource_id = $resourceId");
            $schedule = "";
            while($row = $name->fetch_assoc()){
                $schedule = $row['name'];
            }
            $schedule .= "\n";
            $seriesIds = mysqli_query($mysqli, "SELECT series_id FROM reservation_resources WHERE resource_id = $resourceId AND series_id NOT IN (SELECT series_id FROM reservation_series WHERE status_id = 2)");
            while($row = $seriesIds->fetch_assoc()){
                $reservationInstanceIds = mysqli_query($mysqli, "SELECT reservation_instance_id FROM reservation_instances WHERE series_id = $row[series_id] AND DATE(CONVERT_TZ(start_date,'+0:00','SYSTEM')) <= DATE(NOW()) AND DATE(CONVERT_TZ(end_date,'+0:00','SYSTEM')) >= DATE(NOW())");
                while ($resInstanceId = $reservationInstanceIds->fetch_assoc()) {
                    $reservationsInfo = mysqli_query($mysqli, "SELECT * FROM (SELECT CONVERT_TZ(start_date,'+00:00','SYSTEM'),CONVERT_TZ(end_date,'+00:00','SYSTEM') FROM reservation_instances WHERE reservation_instance_id = $resInstanceId[reservation_instance_id]) AS A JOIN(SELECT title FROM reservation_series WHERE series_id = $row[series_id]) AS B");
                    while ($reservationInfo = $reservationsInfo->fetch_assoc()) {
                        $schedule .= $reservationInfo['title'];
                        $schedule .= "\n";
                        $schedule .= $reservationInfo["CONVERT_TZ(start_date,'+00:00','SYSTEM')"];
                        $schedule .= "\n";
                        $schedule .= $reservationInfo["CONVERT_TZ(end_date,'+00:00','SYSTEM')"];
                        $schedule .= "\n";
                    }
                }
            }
            return $schedule;
        }
        public Function getImage($config, $macAddress, $voltage, $device) {
            $date=`date -d "+4 minutes" +%Y-%m-%d`;
            $time=`date -d "+4 minutes" +%H:%M`;
            $macAddressInfoFilePath = "image_data/" . $macAddress . ".info";
            $macAddressInfo = $macAddress;
            $macAddressInfo .= "\n";
            $macAddressInfo .= $date;
            $macAddressInfo .= $time;
            $macAddressInfo .= $device["device_type"];
            $macAddressInfo .= "\n";
            $macAddressInfo .= $voltage;
            $macAddressInfo .= "\n";
            $macAddressInfo .= $device["orientation"];
            $macAddressInfo .= "\n";
            $macAddressInfo .= $device["resource_id"];
            $macAddressInfo .= "\n";
            $macAddressInfo .= $config->bookedDisplayUrl;
            $macAddressInfo .= "\n";
            $macAddressInfo .= $config->bookedQrCodeBaseUrlBeginning;
            $macAddressInfo .= "\n";
            $macAddressInfo .= $config->bookedQrCodeBaseUrlEnd;
            $macAddressInfo .= "\n";
            $macAddressInfo .= $this->getSchedule($config, $device["resource_id"]);
            $macAddressInfoFile = fopen("$macAddressInfoFilePath", "w") or die("Unable to open file");
            fwrite($macAddressInfoFile, $macAddressInfo);
            `./genimg $macAddressInfoFilePath`;
            $imagePath = "./image_data/" . $macAddress . ".compressed";
            return $imagePath;
        }
    }
    $booked = new bookedPlugin;
    $plugins[ $booked->getIndex() ] = $booked;
}
?>
