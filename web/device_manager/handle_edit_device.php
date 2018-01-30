<?php

    #Debug stuff
    #require_once("print_info.php");
    #printInfo($_POST);

    include 'dbconfig.php';
    echo $_POST["mac_address"];
    echo $_POST["resource_id"];
    echo $_POST["orientation"];
    echo $_POST["device_type"];
    $mysqli = mysqli_connect($server, $username, $password, "door-display");
    $sql_query="UPDATE devices SET resource_id = $_POST[new_resource_id], orientation = $_POST[new_orientation], device_type = $_POST[new_device_type] WHERE mac_address = \"$_POST[new_mac_address]\"";
    if ($mysqli->query($sql_query) === TRUE) {
        header( "refresh: 5; url=/device_manager/view_devices.php");
        echo "Record updated successfully; redirecting in 5 seconds";
    } else {
        echo "Error updating record: " . $mysqli->error;
    }
?>
