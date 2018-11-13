<?php
    #Debug stuff
    if ($config->debugModeActive == 'true') {
        error_reporting(E_ALL);
        ini_set('display_errors', '1');
    }

    $mac_address = $_GET['mac_address'];
    $png = "$_SERVER[DOCUMENT_ROOT]/image_data/" . $mac_address . ".png";
    if (preg_match('/^[[:xdigit:]]+$/', $mac_address) === 1) {
        include "$_SERVER[DOCUMENT_ROOT]/config/dbconfig.php";
        $mysqli = mysqli_connect($config->deviceDatabaseServer, $config->deviceDatabaseUsername, $config->deviceDatabasePassword, $config->deviceDatabaseName);
        $result = mysqli_query($mysqli, "SELECT * FROM devices WHERE mac_address = \"$mac_address\"");
        if ($result->numRows == 0) {
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
        $device['scheduling_system'] = $_GET['scheduling_system'];
        foreach ($plugins as $plugin) {
            if ($plugin->getIndex() == $device['scheduling_system']) {
                $device['orientation'] = 0;
                $device['resource_id'] = $_GET['resource_id'];
                $device['device_type'] = $_GET['layout'];
                $device['width'] = $_GET['width'];
                $device['height'] = $_GET['height'];
                $compressedFile = $plugin->getImage($config, $device);
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
