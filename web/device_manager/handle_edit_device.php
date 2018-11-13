<?php

    #Debug stuff
    #require_once("print_info.php");
    #printInfo($_POST);

    $mac = strtoupper(str_replace(":","",$_POST["new_mac_address"]));
    include '../config/dbconfig.php';
    $mysqli = mysqli_connect($config->deviceDatabaseServer, $config->deviceDatabaseUsername, $config->deviceDatabasePassword, $config->deviceDatabaseName);
    if ($_POST["new_device_id"] == "new") {
        $sql_query="INSERT INTO devices(mac_address,resource_id,orientation,device_type,scheduling_system,is_production,width,height,notes) VALUES (\"$mac\",\"$_POST[new_resource_id]\",$_POST[new_orientation],$_POST[new_device_type],$_POST[new_scheduling_system],$_POST[new_production],$_POST[new_width],$_POST[new_height],\"$_POST[new_notes]\")";
    } else {
        $sql_query="UPDATE devices SET mac_address = \"$mac\", resource_id = \"$_POST[new_resource_id]\", orientation = $_POST[new_orientation], scheduling_system = $_POST[new_scheduling_system], device_type = $_POST[new_device_type], is_production = $_POST[new_production], width = $_POST[new_width], height = $_POST[new_height], notes = '$_POST[new_notes]' WHERE device_id = $_POST[new_device_id]";
    }
    if ($mysqli->query($sql_query) === TRUE) {
        header( "refresh: 3; url=view_devices.php");
        echo "Record updated successfully; redirecting in 3 seconds";
    } else {
        echo "Error updating record: " . $mysqli->error;
        echo "<br>";
        echo $sql_query;
    }
?>
