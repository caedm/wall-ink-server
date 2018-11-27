<?php
    include "$_SERVER[DOCUMENT_ROOT]/config/dbconfig.php";
    #Debug stuff
    if ($config->debugModeActive == 'true') {
        error_reporting(E_ALL);
        ini_set('display_errors', '1');
    }

    $mac_address = $_GET['mac_address'];
    $width = $_GET["width"];
    $height = $_GET["height"];
    $layout = $_GET["layout"];
    $resourceId = $_GET["resource_id"];
    $schedulingSystem = $_GET["scheduling_system"];

    if (preg_match('/^[[:xdigit:]]{1,50}$/', $mac_address) === 1 && preg_match('/^[[:digit:]]{1,50}$/', $width) === 1 && preg_match('/^[[:digit:]]{1,50}$/', $height) === 1 && preg_match('/^[[:digit:]]{1,50}$/', $layout) === 1 && preg_match('/^[[:alphanum:]]{1,100}$/', $resourceId) === 1 && preg_match('/^[[:digit:]]{1,50}$/', $schedulingSystem) === 1) {
        $png = "$_SERVER[DOCUMENT_ROOT]/image_data/" . $mac_address . ".png";
        $mysqli = mysqli_connect($config->deviceDatabaseServer, $config->deviceDatabaseUsername, $config->deviceDatabasePassword, $config->deviceDatabaseName);
        $result = mysqli_query($mysqli, "SELECT * FROM devices WHERE mac_address = \"$mac_address\"");
        if ($result->num_rows == 0) {
            $device = array();
            $device['mac_address'] = $mac_address;
            $device['voltage'] = 7;
            $device['firmware_version'] = "png";
        } else {
            $device = $result->fetch_assoc();
        }
        foreach (glob("$_SERVER[DOCUMENT_ROOT]/plugins/*.php") as $filename) {
            require_once($filename);
        }
        $device['scheduling_system'] = $schedulingSystem;
        foreach ($plugins as $plugin) {
            if ($plugin->getIndex() == $device['scheduling_system']) {
                $device['orientation'] = 0;
                $device['resource_id'] = $resourceId;
                $device['device_type'] = $layout;
                $device['width'] = $width;
                $device['height'] = $height;
                $processedFile = $plugin->getImage($config, $device);
            }
        }
        $raw = "$_SERVER[DOCUMENT_ROOT]/image_data/" . $mac_address;
        `./rawToPng.sh $raw $device[width] $device[height]`;
        if (file_exists($png)) {
            header('Content-Type: image/png');
            header('Cache-Control: no-store, no-cache, must-revalidate, max-age=0');
            header('Cache-Control: post-check=0, pre-check=0', false);
            header('Pragma: no-cache');
            header('Pragma: public');
            readfile($png);
            exit;
        } else {
            echo "error: image not found";
        }
    } else {
        echo "REGEX doesn't match";
    }
?>
