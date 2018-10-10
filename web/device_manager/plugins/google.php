<?php
require_once('../config/dbconfig.php');
require_once('iPlugin.php');
if ($googleIntegrationActive == "true") {
    class googlePlugin implements iPlugin {
        public function getIndex() {
            return 1;
        } 
        public function getName() {
            return "Google Calendar";
        } 
        public function isActive() {
            return $googleIntegrationActive;
        }
        public function getResources() {
            require_once('../google/quickstart.php');
            $rooms = array();
            foreach ($calendarList->getItems() as $calendarListEntry) {
                $rooms[ strtok($calendarListEntry->getID(),"@") ] = $calendarListEntry->getSummary();
            }
            return $rooms;
        }
    }
    $google = new googlePlugin;
    $plugins[ $google->getIndex() ] = $google;
}
?>
