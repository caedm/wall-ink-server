<?php
require('config/dbconfig.php');
#Debug stuff
if ($config->debugModeActive == "true") {
    error_reporting(E_ALL);
    ini_set('display_errors', '1');
}

$mysqli = mysqli_connect($config->deviceDatabaseServer, $config->deviceDatabaseUsername, $config->deviceDatabasePassword, $config->deviceDatabaseName);

//sanitize inputs
$mac_address = $mysqli->real_escape_string($_GET["mac_address"]);
$voltage = $mysqli->real_escape_string($_GET["voltage"]);
$firmware_version = $mysqli->real_escape_string($_GET["firmware"]);
$errorCode = $mysqli->real_escape_string($_GET["error"]);
$width = $mysqli->real_escape_string($_GET["width"]);
$height = $mysqli->real_escape_string($_GET["height"]);

function returnError($plugins,$config,$mac_address, $voltage, $firmware_version, $width, $height, $error) {
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
    $device["plugin"] = "4";
    $device["height"] = $height;
    $device["width"] = $width;
    $device["error_message"] = $error;	

    //Use the simple text plugin to display unknown screen size message
    //Make sure that the simple_text plugin is loaded before calling this function
    //It is not possible to load the simple_text plugin inside this function

    foreach ($plugins as $plugin) {
        if ($plugin->getIndex() == $device["plugin"]) {
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

//get additional info

$result = mysqli_query($mysqli, "SELECT * FROM devices WHERE mac_address = \"$mac_address\"");
// If mac address is known:
if ($result->num_rows != 0) {
    //get device settings from device manager database
    $device = $result->fetch_assoc();
    //Do not update device manager database OR logs if device firmware version is set to "no_update"
    if ($firmware_version != "no_update") { 
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


        // Log contact from a wall-ink device to a log file
        $logfile = "log/" . $mac_address . ".log";
        $log_entry = date('Y-m-d H:i:s') . " MAC Address: " . $mac_address . " Firmware: " . $firmware_version . " Width: " . $width . " Height: " . $height . " Voltage: " . $voltage . " Error Code: " . $errorCode . " Plugin: " . $device["plugin"] . " Resource: " . $device["resource_id"] . " Device Type: " . $device["device_type"] . "\n";
        file_put_contents($logfile, $log_entry, FILE_APPEND | LOCK_EX);
    } // end of "no_update" if statement

    //load ALL plugin code
    foreach (glob("./plugins/*.php") as $filename) {
        require_once($filename);
    }
    //use selected plugin to build wink file
    foreach ($plugins as $plugin) {
        if ($plugin->getIndex() == $device["plugin"]) {
            $file = $plugin->getImage($config, $device);
        }
    }

    //if the plugin successuflly returned the name of a file 
    if (isset($file)) {	
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
        //run this code if the plugin could not successfully return a filename from the selected plugin
    } else {
        //Plugins were already loaded above, so no need to load simple_text plugin at this point.
        returnError($plugins,$config,$mac_address, $voltage, $firmware_version, $width, $height, "Plugin with index " . $device["plugin"] . " failed to run.\nSee web server logs.");
    }
} else {
    // this code block gets executed if the mac address doesn't match an existing device in the device database
    //Load the simple_text plugin and display error to wall-ink device
    require_once("./plugins/simple_text.php");
    returnError($plugins,$config, $mac_address, $voltage, $firmware_version, $width, $height, "Unknown MAC address");
}
?>
