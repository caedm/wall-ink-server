<style>
<?php include 'css/view_devices.css'; ?>
</style>
<?php
function printResult($devices, $rooms) {
    //Display how many results there were
    #echo "$res->num_rows entries<br>";

    //Display each row
    $devices->data_seek(0);
    echo "<table id=\"devices\" class=\"devices\">";

    echo "<tr class=\"headers\">";

    #echo "<th class=\"device_id\">";
    #echo "device_id";
    #echo "</th>";
    echo "<th class=\"mac_address\" onclick=\"sortTable(0)\">";
    echo "MAC Address";
    echo "</th>";
    echo "<th class=\"room_name\" onclick=\"sortTable(1)\">";
    echo "Room Name";
    echo "</th>";
    echo "<th class=\"device_type\" onclick=\"sortTable(2)\">";
    echo "Device Type";
    echo "</th>";
    echo "<th class=\"voltage\" onclick=\"sortTable(3)\">";
    echo "Voltage";
    echo "</th>";
    echo "<th class=\"orientation\" onclick=\"sortTable(4)\">";
    echo "Orientation";
    echo "</th>";
    echo "<th class=\"firmware_version\" onclick=\"sortTable(5)\">";
    echo "Firmware Version";
    echo "</th>";
    echo "<th class=\"last_checked_in\" onclick=\"sortTable(6)\">";
    echo "Last Checked In";
    echo "</th>";
    echo "<th class=\"batteries_replaced_date\" onclick=\"sortTable(7)\">";
    echo "Batteries Replaced Date";
    echo "</th>";

    echo "</tr>";
    
    while ($device = $devices->fetch_assoc()) {
        echo "<tr class=\"device\" onclick=\"document.location = 'edit_device.php?device_id=$device[device_id]'\">";

        #echo "<td class=\"device_id\">";
        #echo $row["device_id"];
        #echo "</td>";
        echo "<td class=\"mac_address\">";
        echo $device["mac_address"];
        echo "</td>";
        echo "<td class=\"room_name\">";
        echo $rooms[$device["resource_id"]];
        echo "</td>";
        echo "<td class=\"device_type\">";
        echo $device["device_type"];
        echo "</td>";
        echo "<td class=\"voltage";
        if ($device["voltage"] < 2.5) {
            echo " low-voltage";
        }
        echo "\">";
        echo $device["voltage"];
        echo "</td>";
        echo "<td class=\"orientation\">";
        echo $device["orientation"];
        echo "</td>";
        echo "<td class=\"firmware_version\">";
        echo $device["firmware_version"];
        echo "</td>";
        echo "<td class=\"last_checked_in\">";
        echo $device["last_checked_in"];
        echo "</td>";
        echo "<td class=\"batteries_replaced_date\">";
        echo $device["batteries_replaced_date"];
        echo "</td>";

        echo "</tr>";
    }
    echo "<tr class=\"fake_device\" onclick=\"document.location = 'edit_device.php?device_id=new'\">";
    echo "<td>Add New Device</td>";
    echo "<td></td>";
    echo "<td></td>";
    echo "<td></td>";
    echo "<td></td>";
    echo "<td></td>";
    echo "<td></td>";
    echo "<td></td>";
    echo "</tr>";
    echo "<tr class=\"fake_device\" onclick=\"document.location = 'voltage_charts.php'\">";
    echo "<td>Battery History</td>";
    echo "<td></td>";
    echo "<td></td>";
    echo "<td></td>";
    echo "<td></td>";
    echo "<td></td>";
    echo "<td></td>";
    echo "<td></td>";
    echo "</tr>";

    echo "</table>";
}

#require_once("print_info.php");
#echo phpversion();
#error_reporting(E_ALL);
#ini_set('display_errors', '1');
include '../config/dbconfig.php';
$mysqli = mysqli_connect($server, $username, $password, "door-display");
$devices = mysqli_query($mysqli, "SELECT * FROM devices");
$mysqli = mysqli_connect($server, $username, $password, "collegeresv");
$resources = mysqli_query($mysqli, "SELECT resource_id,name FROM resources");
$rooms = array();
while($room = $resources->fetch_assoc()){
   $rooms[ $room["resource_id"] ] = $room["name"];
}
include '../google/quickstart.php';
foreach ($calendarList->getItems() as $calendarListEntry) {
    $rooms[ strtok($calendarListEntry->getID(),"@") ] = $calendarListEntry->getSummary();
}
echo "<script src='js/view_devices.js'></script>";
printResult($devices, $rooms);
?>

