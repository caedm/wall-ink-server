<?php
interface iPlugin {
    public function getIndex();
    public function getName();
    public function isActive();
    public function getResources();
}
$plugins = array();
?>
