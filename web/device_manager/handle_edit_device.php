<?php

    #Debug stuff
    #require_once("print_info.php");
    #printInfo($_POST);

    include 'dbconfig.php';
    $mysqli = mysqli_connect($server, $username, $password, "door-display");
    if ($_POST["new_device_id"] == "new") {
        $sql_query="INSERT INTO devices(mac_address,resource_id,orientation,device_type) VALUES (\"$_POST[new_mac_address]\",$_POST[new_resource_id],$_POST[new_orientation],$_POST[new_device_type])";
    } else {
        $sql_query="UPDATE devices SET mac_address = \"$_POST[new_mac_address]\", resource_id = $_POST[new_resource_id], orientation = $_POST[new_orientation], device_type = $_POST[new_device_type] WHERE device_id = $_POST[new_device_id]";
    }
    if ($mysqli->query($sql_query) === TRUE) {
        header( "refresh: 5; url=/device_manager/view_devices.php");
        echo "Record updated successfully; redirecting in 5 seconds";
    } else {
        echo "Error updating record: " . $mysqli->error;
        echo "<br>";
        echo $sql_query;
    }
?>
