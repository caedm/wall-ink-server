<?php
    require('config/dbconfig.php');
    #Debug stuff
    if ($config->debugModeActive == "true") {
        error_reporting(E_ALL);
        ini_set('display_errors', '1');
    }

    $mac_address = $_GET["mac_address"];
    $voltage = $_GET["voltage"];
    $firmware_version = $_GET["firmware"];
    $errorCode = $_GET["error"];
    if (preg_match('/^[[:xdigit:]]+$/', $mac_address) === 1 && preg_match('/^[[:digit:].]+$/', $voltage) === 1 && preg_match('/^[0-9a-z._]*$/', $firmware_version) === 1 && preg_match('/^[[:digit:]]*$/', $errorCode) === 1) {

        //get additional info
        $mysqli = mysqli_connect($config->deviceDatabaseServer, $config->deviceDatabaseUsername, $config->deviceDatabasePassword, $config->deviceDatabaseName);
        $result = mysqli_query($mysqli, "SELECT * FROM devices WHERE mac_address = \"$mac_address\"");
        $device = $result->fetch_assoc();
        
        //update firmware version
        $sql_query="UPDATE devices SET firmware_version = \"$firmware_version\" WHERE mac_address = \"$mac_address\"";
        $result = mysqli_query($mysqli, $sql_query);

        $file = "image_data/" . $mac_address . ".static";
        if ($device["voltage"] + 0.35 < "$voltage"){
            $sql_query="UPDATE devices SET batteries_replaced_date = NOW() WHERE mac_address = \"$mac_address\"";
            $result = mysqli_query($mysqli, $sql_query);
        }
        $sql_query="UPDATE devices SET voltage = $voltage, last_checked_in = NOW() WHERE mac_address = \"$mac_address\"";
        $result = mysqli_query($mysqli, $sql_query);
        if ($device["device_type"] == 5 || $device["device_type"] == 8) {
            $file = "image_data/" . $mac_address . ".static";
        } else {
            foreach (glob("./plugins/*.php") as $filename) {
                require_once($filename);
            }
            foreach ($plugins as $plugin) {
                if ($plugin->getIndex() == $device["scheduling_system"]) {
                    $file = $plugin->getImage($config, $device);
                }
            }
        }
        if (file_exists($file)) {
            header('Content-Description: File Transfer');
            header('Content-Type: application/octet-stream');
            header('Content-Disposition: attachment; filename="'.basename($file).'"');
            header('Expires: 0');
            header("Cache-Control: no-store, no-cache, must-revalidate, max-age=0");
            header("Cache-Control: post-check=0, pre-check=0", false);
            header("Pragma: no-cache");
            header('Pragma: public');
            header('Content-Length: ' . filesize($file));
            readfile($file);
            exit;
        }
    } else {
        echo "REGEX doesn't match";
    }
?>
