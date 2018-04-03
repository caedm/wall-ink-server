<?php
function printResult($devices, $rooms) {
    //Display how many results there were
    #echo "$res->num_rows entries<br>";

    //Display each row
    $devices->data_seek(0);

    while ($device = $devices->fetch_assoc()) {
        echo "<h4>$device[mac_address]</h4>";
        echo "<img src=\"/voltage_monitor/data/$device[mac_address].png\">";
    }

}

#require_once("print_info.php");
#echo phpversion();
error_reporting(E_ALL);
ini_set('display_errors', '1');
include 'dbconfig.php';
$mysqli = mysqli_connect($server, $username, $password, "door-display");
$devices = mysqli_query($mysqli, "SELECT * FROM devices");
$mysqli = mysqli_connect($server, $username, $password, "collegeresv");
$resources = mysqli_query($mysqli, "SELECT resource_id,name FROM resources");
$rooms = array();
while($room = $resources->fetch_assoc()){
   $rooms[ $room["resource_id"] ] = $room["name"];
}
printResult($devices, $rooms);
?>

