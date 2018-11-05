<?php
require_once("$_SERVER[DOCUMENT_ROOT]/plugin_dependencies/iPlugin.php");
require("$_SERVER[DOCUMENT_ROOT]/config/dbconfig.php");
if ($config->staticImagesPluginActive == "true") {
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
        public function getSchedule($config, $resourceId) {
            return $resourceId;
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

            #$sourceImage = "";
            #foreach ($images as $image) {
                #$sourceImage = $image;
            #}
            $timeIncrement = 300;
            $nextRefreshTime = $timeIncrement - ($_SERVER['REQUEST_TIME'] % $timeIncrement);
            #$images = array_values($images);
            #sort($images);
            $sourceImage = $images[floor($_SERVER['REQUEST_TIME'] / $timeIncrement) % count($images)];
            $pbm = "$_SERVER[DOCUMENT_ROOT]/image_data/" . $device["mac_address"] . "." . "pbm";
            $raw = "$_SERVER[DOCUMENT_ROOT]/image_data/" . $device["mac_address"];
            $static = "$_SERVER[DOCUMENT_ROOT]/image_data/" . $device["mac_address"] . "." . "static";
            `convert $sourceImage -rotate 180 -negate -resize $size\! $pbm`;
            `$_SERVER[DOCUMENT_ROOT]/pbmToRaw.sh $pbm $raw`;
            `$_SERVER[DOCUMENT_ROOT]/rawToCompressed $raw $static $width $height $nextRefreshTime`;
            return $static;
        }
        public function getDeviceType($device) {
            require("$_SERVER[DOCUMENT_ROOT]/plugin_dependencies/general_scheduling/schedulingGetDeviceType.php");
            return schedulingGetDeviceType($device);
        }
    }
    $staticImages = new staticImagesPlugin;
    $plugins[ $staticImages->getIndex() ] = $staticImages;
}
?>
