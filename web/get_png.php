<?php
include "$_SERVER[DOCUMENT_ROOT]/config/dbconfig.php";
#Debug stuff
if ($config->debugModeActive == 'true') {
    error_reporting(E_ALL);
    ini_set('display_errors', '1');
}

$mysqli = mysqli_connect($config->deviceDatabaseServer, $config->deviceDatabaseUsername, $config->deviceDatabasePassword, $config->deviceDatabaseName);

$mac_address = $mysqli->real_escape_string($_GET['mac_address']);
$width = $mysqli->real_escape_string($_GET["width"]);
$height = $mysqli->real_escape_string($_GET["height"]);
$layout = $mysqli->real_escape_string($_GET["layout"]);
$resourceId = $mysqli->real_escape_string($_GET["resource_id"]);
$plugin = $mysqli->real_escape_string($_GET["plugin"]);

$png = "$_SERVER[DOCUMENT_ROOT]/image_data/" . $mac_address . ".png";
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
$device['plugin'] = $plugin;
foreach ($plugins as $plugin) {
    if ($plugin->getIndex() == $device['plugin']) {
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
?>
