<?php
require_once('../config/dbconfig.php');
require_once("iPlugin.php");
if ($bookedIntegrationActive == "true") {
    class bookedPlugin implements iPlugin {
        public function getIndex() {
            return 0;
        } 
        public function getName() {
            return "Booked";
        } 
        public function isActive() {
            return $bookedIntegrationActive;
        }
        public function getResources() {
            require('../config/dbconfig.php');
            $rooms = array();
            $mysqli = mysqli_connect($bookedDatabaseServer, $bookedDatabaseUsername, $bookedDatabasePassword, $bookedDatabaseName);
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
