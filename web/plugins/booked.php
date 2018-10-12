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
    }
    $booked = new bookedPlugin;
    $plugins[ $booked->getIndex() ] = $booked;
}
?>
