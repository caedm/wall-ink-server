<?php
interface iPlugin {
    public function getIndex();
    public function getName();
    public function isActive($config);
    public function getResources($config);
}
$plugins = array();
?>
