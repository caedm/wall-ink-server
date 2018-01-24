<?php
    function display($string) {
        echo "$string<br>";

    }   

    // Print out everything on an object
    function printInfo($object, $tag = "") {
        if (is_resource($object)) {
            display($tag . "Is a resource");
        } else if (is_integer($object)) {
            display($tag . "(int): $object");
        } else if (is_float($object)) {
            display($tag . "(float): $object");
        } else if (is_string($object)) {
            display($tag . "(str): $object");
        } else if (is_bool($object)) {
            if ($object) {
                display($tag . "(bol): TRUE");
            } else {
                display($tag . "(bol): FALSE");
            }
        } else if (is_null($object)) {
            display($tag . "(null): NULL");
        } else if (is_array($object)) {
            printArray($object, $tag);
        } else {
            printClass($object, $tag);
        }
    }   

    function printArray($stuff, $tag = "") {
        if (count($stuff) === 0) {
            display($tag . "Empty Array");
            return;
        }
        foreach ($stuff as $key => $value) {
            printInfo($value, $tag . "[$key] ");
        }
    }   

    function printClass($class, $tag = "") {
        foreach (get_class_methods(get_class($class)) as $method) {
            display($tag . "METHOD: $method");
        }
        foreach (get_object_vars($class) as $key => $value) {
            printInfo($value, $tag . '{' . $key .'} ');
        }
    }   
?>
