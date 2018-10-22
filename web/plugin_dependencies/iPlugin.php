<?php
interface iPlugin {
    public function getIndex();
    public function getName();
    public function isActive($config);
    public function getResources($config);
    public function getSchedule($config, $resourceId);
    public function getImage($config, $device);
}
$plugins = array();
?>
