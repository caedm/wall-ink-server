<?php
require('config/dbconfig.php');
#Debug stuff
if ($config->debugModeActive == "true") {
    error_reporting(E_ALL);
    ini_set('display_errors', '1');
}

$mysqli = mysqli_connect($config->deviceDatabaseServer, $config->deviceDatabaseUsername, $config->deviceDatabasePassword, $config->deviceDatabaseName);
$plugin_id = $mysqli->real_escape_string($_GET["plugin_id"]);
$resource_id = $mysqli->real_escape_string($_GET["resource_id"]);

$resource_name = "";
foreach (glob("$_SERVER[DOCUMENT_ROOT]/plugins/*.php") as $filename) {
    require_once($filename);
}
foreach ($plugins as $plugin) {
    if ($plugin->getIndex() == $plugin_id) {
        $resources = $plugin->getResources($config);
        $resource = array();
        $resource['resource_name'] = $resources[$resource_id];
        echo json_encode($resource);
    }
}
?>
