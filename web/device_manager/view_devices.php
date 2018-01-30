<style>
<?php include 'css/view_devices.css'; ?>
</style>
<?php
function printResult($devices) {
    //Display how many results there were
    #echo "$res->num_rows entries<br>";

    //Display each row
    $devices->data_seek(0);
    echo "<table class=\"devices\">";

    echo "<tr class=\"headers\">";

    #echo "<th class=\"device_id\">";
    #echo "device_id";
    #echo "</th>";
    echo "<th class=\"mac_address\">";
    echo "mac_address";
    echo "</th>";
    echo "<th class=\"resource_id\">";
    echo "resource_id";
    echo "</th>";
    echo "<th class=\"device_type\">";
    echo "device_type";
    echo "</th>";
    echo "<th class=\"voltage\">";
    echo "voltage";
    echo "</th>";
    echo "<th class=\"orientation\">";
    echo "orientation";
    echo "</th>";
    echo "<th class=\"last_checked_in\">";
    echo "last_checked_in";
    echo "</th>";
    echo "<th class=\"batteries_replaced_date\">";
    echo "batteries_replaced_date";
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
        echo "<td class=\"resource_id\">";
        echo $device["resource_id"];
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
        echo "<td class=\"last_checked_in\">";
        echo $device["last_checked_in"];
        echo "</td>";
        echo "<td class=\"batteries_replaced_date\">";
        echo $device["batteries_replaced_date"];
        echo "</td>";

        echo "</tr>";
    }
    echo "<tr class=\"device\" onclick=\"document.location = 'edit_device.php?device_id=new'\">";
    echo "<td>Add New Device</td>";
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
error_reporting(E_ALL);
ini_set('display_errors', '1');
include 'dbconfig.php';
$mysqli = mysqli_connect($server, $username, $password, "door-display");
$devices = mysqli_query($mysqli, "SELECT * FROM devices");
printResult($devices);
?>

