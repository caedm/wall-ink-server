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
        public function getImage($config, $device) {
            require("$_SERVER[DOCUMENT_ROOT]/plugin_dependencies/general_scheduling/schedulingGetImage.php");
            return schedulingGetImage($config, $device, $this->getSchedule($config, $device["resource_id"]), $config->googleDisplayUrl, $config->googleQrCodeBaseUrlBeginning, $config->googleQrCodeBaseUrlEnd);
        }
        public function getDeviceType($device) {
            require("$_SERVER[DOCUMENT_ROOT]/plugin_dependencies/general_scheduling/schedulingGetDeviceType.php");
            return schedulingGetDeviceType($device);
        }
    }
    $google = new googlePlugin;
    $plugins[ $google->getIndex() ] = $google;
}
?>
