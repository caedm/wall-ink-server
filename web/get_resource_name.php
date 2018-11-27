<?php
    require('config/dbconfig.php');
    #Debug stuff
    if ($config->debugModeActive == "true") {
        error_reporting(E_ALL);
        ini_set('display_errors', '1');
    }

    $plugin_id = $_GET["plugin_id"];
    $resource_id = $_GET["resource_id"];

    //Sanity check on incoming url 
    if (preg_match('/^[[:xdigit:]]{1,50}$/', $plugin_id) === 1 && preg_match('/^[[:alphanum:]]{1,100}$/', $resourceId) === 1) {
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
    } else {
        echo "REGEX doesn't match";
    }
?>
