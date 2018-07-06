<style>
<?php include 'css/voltage_charts.css'; ?>
</style>
<?php
function printResult($devices, $rooms) {
    //Display how many results there were
    #echo "$res->num_rows entries<br>";

    //Display each row
    $devices->data_seek(0);

    echo "<a href=\"view_devices.php\"><h4>Home</h4></a>";

    while ($device = $devices->fetch_assoc()) {
        echo "<h4>$device[mac_address] - ";
        echo $rooms[$device["resource_id"]];
        echo "</h4>";
        echo "<div>";
        echo "<img src=\"/voltage_monitor/data/week_$device[mac_address].png\">";
        echo "<img src=\"/voltage_monitor/data/month_$device[mac_address].png\">";
        echo "<img src=\"/voltage_monitor/data/year_$device[mac_address].png\">";
        echo "</div>";
    }

}

#require_once("print_info.php");
#echo phpversion();
error_reporting(E_ALL);
ini_set('display_errors', '1');
include '../config/dbconfig.php';
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

