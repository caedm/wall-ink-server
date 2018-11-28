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
$width = $_GET["width"];
$height = $_GET["height"];

function returnError($mac_address, $voltage, $firmware_version, $width, $height, $error) {
    //Since there is no database entry for this device
    //Set to use the simple text plugin
    //Get as much information from the device query as possible to 
    //pass into the the plugin 
    $device["resource_id"] = "error";
    $device["device_type"] = "30";
    $device["mac_address"] = $mac_address;
    $device["voltage"] = $voltage;
    $device["orientation"] = "1";
    $device["firmware_version"] = $firmware_version;
    $device["scheduling_system"] = "4";
    $device["height"] = $height;
    $device["width"] = $width;
    $device["error_message"] = $error;	

    //use the simple text plugin to display unknown screen size message
    require_once("./plugins/simple_text.php");

    foreach ($plugins as $plugin) {
        if ($plugin->getIndex() == $device["scheduling_system"]) {
            $file = $plugin->getImage($config, $device);
        }
    }

    if (file_exists($file)) {
        header('Content-Description: File Transfer');
        header('Content-Type: application/octet-stream');
        header('Content-Disposition: attachment; filename="'.basename($file).'"');
        header('Expires: 0');
        header('Cache-Control: no-store, no-cache, must-revalidate, max-age=0');
        header('Cache-Control: post-check=0, pre-check=0', false);
        header('Pragma: no-cache');
        header('Pragma: public');
        header('Content-Length: ' . filesize($file));
        readfile($file);
        exit;
    }
}

//Sanity check on incoming url 
if (preg_match('/^[[:xdigit:]]{1,50}$/', $mac_address) === 1 && preg_match('/^[[:digit:].]{1,50}$/', $voltage) === 1 && preg_match('/^[[:alphanum:]._]{1,50}$/', $firmware_version) === 1 && preg_match('/^[[:digit:]]{1,50}$/', $errorCode) === 1 && preg_match('/^[[:digit:]]{1,50}$/', $width) === 1 && preg_match('/^[[:digit:]]{1,50}$/', $height) === 1) {

    //get additional info
    $mysqli = mysqli_connect($config->deviceDatabaseServer, $config->deviceDatabaseUsername, $config->deviceDatabasePassword, $config->deviceDatabaseName);

    $result = mysqli_query($mysqli, "SELECT * FROM devices WHERE mac_address = \"$mac_address\"");
    // If mac address is known:
    if ($result->num_rows != 0) {
        //get device settings from device manager database
        $device = $result->fetch_assoc();

        //update firmware version
        $sql_query="UPDATE devices SET firmware_version = \"$firmware_version\" WHERE mac_address = \"$mac_address\"";
        $result = mysqli_query($mysqli, $sql_query);

        //update batteries replaced date if voltage is significantly higher than last time 
        if ($device["voltage"] + 0.35 < "$voltage"){
            $sql_query="UPDATE devices SET batteries_replaced_date = NOW() WHERE mac_address = \"$mac_address\"";
            $result = mysqli_query($mysqli, $sql_query);
        }
        //update voltage
        $sql_query="UPDATE devices SET voltage = $voltage, last_checked_in = NOW() WHERE mac_address = \"$mac_address\"";
        $result = mysqli_query($mysqli, $sql_query);


        foreach (glob("./plugins/*.php") as $filename) {
            require_once($filename);
        }
        foreach ($plugins as $plugin) {
            if ($plugin->getIndex() == $device["scheduling_system"]) {
                $file = $plugin->getImage($config, $device);
            }
        }

        // Log contact from a wall-ink device to a log file
        $logfile = "log/" . $mac_address . ".log";
        $log_entry = date('Y-m-d H:i:s') . " MAC Address: " . $mac_address . " Firmware: " . $firmware_version . " Width: " . $width . " Height: " . $height . " Voltage: " . $voltage . " Error Code: " . $errorCode . " Plugin: " . $device["scheduling_system"] . " Resource: " . $device["resource_id"] . " Device Type: " . $device["device_type"] . "\n";
        file_put_contents($logfile, $log_entry, FILE_APPEND | LOCK_EX);

        if (file_exists($file)) {
            header('Content-Description: File Transfer');
            header('Content-Type: application/octet-stream');
            header('Content-Disposition: attachment; filename="'.basename($file).'"');
            header('Expires: 0');
            header('Cache-Control: no-store, no-cache, must-revalidate, max-age=0');
            header('Cache-Control: post-check=0, pre-check=0', false);
            header('Pragma: no-cache');
            header('Pragma: public');
            header('Content-Length: ' . filesize($file));
            readfile($file);
            exit;
        }
    } else {
        // this code block gets executed if the mac address doesn't match an existing device in the device database
        returnError($mac_address, $voltage, $firmware_version, $width, $height, "Unknown MAC address");
    }
} else {
    // if URL does not pass sanity check:
    echo "REGEX doesn't match";
} 
?>
