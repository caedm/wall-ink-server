<?php
require_once("$config->runTimeWebDirectory/plugin_dependencies/iPlugin.php");
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
    }
    $google = new googlePlugin;
    $plugins[ $google->getIndex() ] = $google;
}
?>
