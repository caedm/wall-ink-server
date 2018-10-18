<?php
require_once("$_SERVER[DOCUMENT_ROOT]/plugin_dependencies/iPlugin.php");
require("$_SERVER[DOCUMENT_ROOT]/config/dbconfig.php");
if ($config->googleIntegrationActive == "true") {
    class googlePlugin implements iPlugin {
        public function getIndex() {
            return 1;
        } 
        public function getName() {
            return "Google Calendar";
        } 
        public function isActive($config) {
            return $config->googleIntegrationActive;
        }
        public function getResources($config) {
            require_once("$config->runTimeWebDirectory/plugin_dependencies/google/quickstart.php");
            $rooms = array();
            foreach ($calendarList->getItems() as $calendarListEntry) {
                $rooms[ strtok($calendarListEntry->getID(),"@") ] = $calendarListEntry->getSummary();
            }
            return $rooms;
        }
        public function getSchedule($config, $resourceId) {
            require_once("$_SERVER[DOCUMENT_ROOT]/plugin_dependencies/google/quickstart.php");
            return google_getSchedule($resourceId);
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
            $macAddressInfo .= $config->googleCalendarDisplayUrl;
            $macAddressInfo .= "\n";
            $macAddressInfo .= $config->googleCalendarQrCodeBaseUrlBeginning;
            $macAddressInfo .= "\n";
            $macAddressInfo .= $config->googleCalendarQrCodeBaseUrlEnd;
            $macAddressInfo .= "\n";
            $macAddressInfo .= $this->getSchedule($config, $device["resource_id"]);
            $macAddressInfoFile = fopen("$macAddressInfoFilePath", "w") or die("Unable to open file");
            fwrite($macAddressInfoFile, $macAddressInfo);
            `./genimg $macAddressInfoFilePath`;
            $imagePath = "./image_data/" . $macAddress . ".compressed";
            return $imagePath;
        }
    }
    $google = new googlePlugin;
    $plugins[ $google->getIndex() ] = $google;
}
?>
