<style>
<?php include 'css/view_devices.css'; ?>
</style>
<?php
function printResult($devices, $rooms, $plugins) {
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
    echo "<th class=\"plugin_name\" onclick=\"sortTable(1)\">";
    echo "Plugin";
    echo "</th>";
    echo "<th class=\"room_name\" onclick=\"sortTable(2)\">";
    echo "Resource";
    echo "</th>";
    echo "<th class=\"device_type\" onclick=\"sortTable(3)\">";
    echo "Device Type";
    echo "</th>";
    echo "<th class=\"voltage\" onclick=\"sortTable(4)\">";
    echo "Voltage";
    echo "</th>";
    echo "<th class=\"orientation\" onclick=\"sortTable(5)\">";
    echo "Orientation";
    echo "</th>";
    echo "<th class=\"firmware_version\" onclick=\"sortTable(6)\">";
    echo "Firmware Version";
    echo "</th>";
    echo "<th class=\"last_checked_in\" onclick=\"sortTable(7)\">";
    echo "Last Checked In";
    echo "</th>";
    echo "<th class=\"batteries_replaced_date\" onclick=\"sortTable(8)\">";
    echo "Batteries Replaced Date";
    echo "</th>";

    echo "</tr>";
    
    while ($device = $devices->fetch_assoc()) {
        echo "<tr class=\"device\" onclick=\"document.location = 'edit_device.php?device_id=$device[device_id]'\">";

        #echo "<td class=\"device_id\">";
        #echo $row["device_id"];
        #echo "</td>";
        echo "<td class=\"mac_address";
        if (!$device['is_production']) {
            echo " notproduction";
        }
        echo "\">";
        echo $device["mac_address"];
        echo "</td>";
        echo "<td class=\"plugin_name";
        if (!$device['is_production']) {
            echo " notproduction";
        }
        echo "\">";
        if (isset($plugins[$device['scheduling_system']])) {
            echo $plugins[$device['scheduling_system']]->getName();
        } else {
            echo "Error: Plugin not active";
        }
        echo "</td>";
        echo "<td class=\"room_name";
        if (!$device['is_production']) {
            echo " notproduction";
        }
        echo "\">";
        if (isset($plugins[$device['scheduling_system']])) {
            echo $rooms[$device['scheduling_system']][$device['resource_id']];
        } else {
            echo "Error: Plugin not active";
        }
        echo "</td>";
        echo "<td class=\"device_type";
        if (!$device['is_production']) {
            echo " notproduction";
        }
        echo "\">";
        echo $device["device_type"];
        echo "</td>";
        echo "<td class=\"voltage";
        if (!$device['is_production']) {
            echo " notproduction";
        }
        if ($device["voltage"] < 2.5) {
            echo " orange";
        }
        echo "\">";
        echo $device["voltage"];
        echo "</td>";
        echo "<td class=\"orientation";
        if (!$device['is_production']) {
            echo " notproduction";
        }
        echo "\">";
        echo $device["orientation"];
        echo "</td>";
        echo "<td class=\"firmware_version";
        if (!$device['is_production']) {
            echo " notproduction";
        }
        echo "\">";
        echo $device["firmware_version"];
        echo "</td>";
        echo "<td class=\"last_checked_in";
        if (substr($device["last_checked_in"], 0, 10) !== date('Y-m-d') && $device['is_production']) {
            echo " orange";
        }
        if (!$device['is_production']) {
            echo " notproduction";
        }
        echo "\">";
        echo $device["last_checked_in"];
        echo "</td>";
        echo "<td class=\"batteries_replaced_date";
        if (!$device['is_production']) {
            echo " notproduction";
        }
        echo "\">";
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

require_once("../config/dbconfig.php");
if ($config->debugModeActive == "true") {
    error_reporting(E_ALL);
    ini_set('display_errors', '1');
}
$mysqli = mysqli_connect($config->deviceDatabaseServer, $config->deviceDatabaseUsername, $config->deviceDatabasePassword, $config->deviceDatabaseName);
$devices = mysqli_query($mysqli, "SELECT * FROM devices");
$rooms = array();
foreach (glob("../plugins/*.php") as $filename) {
    require_once($filename);
}
foreach ($plugins as $plugin) {
    $rooms[ $plugin->getIndex() ] = $rooms + $plugin->getResources($config);
}
echo "<script src='js/view_devices.js'></script>";
printResult($devices, $rooms, $plugins);
?>
