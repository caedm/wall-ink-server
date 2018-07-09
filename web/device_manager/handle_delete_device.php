<?php

    #Debug stuff
    #require_once("print_info.php");
    #printInfo($_GET);

    include '../config/dbconfig.php';
    $mysqli = mysqli_connect($deviceDatabaseServer, $deviceDatabaseUsername, $deviceDatabasePassword, "door-display");
    $sql_query="DELETE FROM devices WHERE device_id = $_GET[device_id]";
    if ($mysqli->query($sql_query) === TRUE) {
        header( "refresh: 3; url=view_devices.php");
        echo "Device deleted successfully; redirecting in 3 seconds";
    } else {
        echo "Error updating record: " . $mysqli->error;
        echo "<br>";
        echo $sql_query;
    }
?>
