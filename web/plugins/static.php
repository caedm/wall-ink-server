<?php
require_once("$_SERVER[DOCUMENT_ROOT]/plugin_dependencies/iPlugin.php");
require("$_SERVER[DOCUMENT_ROOT]/config/dbconfig.php");
class staticImagesPlugin implements iPlugin {
    public function getIndex() {
        return 2;
    }
    public function getName() {
        return "Static Images";
    }
    public function isActive($config) {
        return $config->staticImagesPluginActive;
    }
    public function getResources($config) {
        $configurations = array();
        $directories = glob($config->runTimeWebDirectory . '/plugin_dependencies/static_images' . '/*' , GLOB_ONLYDIR);
        foreach ($directories as $directory) {
            $configurations[ substr($directory, strlen($directory) - strpos(strrev($directory), '/')) ] = substr($directory, strlen($directory) - strpos(strrev($directory), '/'));
        }
        return $configurations;
    }
    public function getImage($config, $device) {
        $size = "";
        if ($device["device_type"] == 1 || $device["device_type"] == 4 || $device["device_type"] == 6 || $device["device_type"] == 9) {
            $size = "400x300";
            $width = 400;
            $height = 300;
        } else if ($device["device_type"] == 0) {
            $size = "384x640";
            $width = 384;
            $height = 640;
        } else {
            $size = "640x384";
            $width = 640;
            $height = 384;
        }
        $imagesDir = $config->runTimeWebDirectory . "/plugin_dependencies/static_images/" . $device["resource_id"] . '/*';
        $images = glob($imagesDir);

        $timeIncrement = 3600;
        if ($device['device_type'] == 1 || $device['device_type'] == 2) {
            $timeIncrement = 1800;
        } else if ($device['device_type'] == 4 || $device['device_type'] == 3) {
            $timeIncrement = 3600;
        } else if ($device['device_type'] == 6 || $device['device_type'] == 7) {
            $timeIncrement = 10800;
        } else if ($device['device_type'] == 9 || $device['device_type'] == 10) {
            $timeIncrement = 86400;
        }
        $nextRefreshTime = $timeIncrement - ($_SERVER['REQUEST_TIME'] % $timeIncrement);
        $sourceImage = $images[floor($_SERVER['REQUEST_TIME'] / $timeIncrement) % count($images)];
        $pbm = "$_SERVER[DOCUMENT_ROOT]/image_data/" . $device["mac_address"] . "." . "pbm";
        $raw = "$_SERVER[DOCUMENT_ROOT]/image_data/" . $device["mac_address"];
        $static = "$_SERVER[DOCUMENT_ROOT]/image_data/" . $device["mac_address"] . "." . "static";
        $angle = 0;
        if ($device['orientation'] == 1) {
            $angle = 180;
        }
        `convert $sourceImage -rotate 180 -negate -resize $size\! $pbm`;
        `$_SERVER[DOCUMENT_ROOT]/pbmToRaw.sh $pbm $raw`;
        `$_SERVER[DOCUMENT_ROOT]/rawToCompressed $raw $static $width $height $nextRefreshTime`;
        return $static;
    }
    public function getDeviceType($device) {
        $getDeviceType = "";
        $getDeviceType .= "<fieldset class=\"field\">";
            $getDeviceType .= "<legend>Device Type</legend>";
            $getDeviceType .= "<ul>";
                $getDeviceType .= "<li>";
                    $getDeviceType .= "<label for=\"1\">4\" static, 30 minute refresh cycle</label>";
                    $getDeviceType .= "<input type=\"radio\" id=\"type_1\" name=\"new_device_type\" value=\"1\"";
                    if ($device['device_type'] == 1) {
                        $getDeviceType .= " checked";
                    }
                    $getDeviceType .= ">";
                $getDeviceType .= "</li>";
                $getDeviceType .= "<li>";
                    $getDeviceType .= "<label for=\"4\">4\" static, 1 hour refresh cycle</label>";
                    $getDeviceType .= "<input type=\"radio\" id=\"type_4\" name=\"new_device_type\" value=\"4\"";
                    if ($device['device_type'] == 4) {
                        $getDeviceType .= " checked";
                    }
                    $getDeviceType .= ">";
                $getDeviceType .= "</li>";
                $getDeviceType .= "<li>";
                    $getDeviceType .= "<label for=\"6\">4\" static, 3 hour refresh cycle</label>";
                    $getDeviceType .= "<input type=\"radio\" id=\"type_6\" name=\"new_device_type\" value=\"6\"";
                    if ($device['device_type'] == 6) {
                        $getDeviceType .= " checked";
                    }
                    $getDeviceType .= ">";
                $getDeviceType .= "</li>";
                $getDeviceType .= "<li>";
                    $getDeviceType .= "<label for=\"9\">4\" static, 1 day refresh cycle</label>";
                    $getDeviceType .= "<input type=\"radio\" id=\"type_9\" name=\"new_device_type\" value=\"9\"";
                    if ($device['device_type'] == 9) {
                        $getDeviceType .= " checked";
                    }
                    $getDeviceType .= ">";
                $getDeviceType .= "</li>";
                $getDeviceType .= "<li>";
                    $getDeviceType .= "<label for=\"2\">7\" static, 30 minute refresh cycle</label>";
                    $getDeviceType .= "<input type=\"radio\" id=\"type_2\" name=\"new_device_type\" value=\"2\"";
                    if ($device['device_type'] == 2) {
                        $getDeviceType .= " checked";
                    }
                    $getDeviceType .= ">";
                $getDeviceType .= "</li>";
                $getDeviceType .= "<li>";
                    $getDeviceType .= "<label for=\"3\">7\" static, 1 hour refresh cycle</label>";
                    $getDeviceType .= "<input type=\"radio\" id=\"type_3\" name=\"new_device_type\" value=\"3\"";
                    if ($device['device_type'] == 3) {
                        $getDeviceType .= " checked";
                    }
                    $getDeviceType .= ">";
                $getDeviceType .= "</li>";
                $getDeviceType .= "<li>";
                    $getDeviceType .= "<label for=\"7\">7\" static, 3 hour refresh cycle</label>";
                    $getDeviceType .= "<input type=\"radio\" id=\"type_7\" name=\"new_device_type\" value=\"7\"";
                    if ($device['device_type'] == 7) {
                        $getDeviceType .= " checked";
                    }
                    $getDeviceType .= ">";
                $getDeviceType .= "</li>";
                $getDeviceType .= "<li>";
                    $getDeviceType .= "<label for=\"10\">7\" static, 1 day refresh cycle</label>";
                    $getDeviceType .= "<input type=\"radio\" id=\"type_10\" name=\"new_device_type\" value=\"10\"";
                    if ($device['device_type'] == 10) {
                        $getDeviceType .= " checked";
                    }
                    $getDeviceType .= ">";
                $getDeviceType .= "</li>";
            $getDeviceType .= "</ul>";
        $getDeviceType .= "</fieldset>";
        return $getDeviceType;
    }
}
if ($config->staticImagesPluginActive == "true") {
    $staticImages = new staticImagesPlugin;
    $plugins[ $staticImages->getIndex() ] = $staticImages;
}
?>
