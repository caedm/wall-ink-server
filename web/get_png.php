<?php
    #Debug stuff
    #require_once("print_info.php");
    #printInfo($_POST);

    #ini_set('display_errors', 1);
    #ini_set('display_startup_errors', 1);
    #error_reporting(E_ALL);

    $mac_address = $_GET["mac_address"];
    if (preg_match('/^[[:xdigit:]]+$/', $mac_address) === 1) {
        include 'device_manager/dbconfig.php';
        $mysqli = mysqli_connect($server, $username, $password, "door-display");
        $result = mysqli_query($mysqli, "SELECT * FROM devices WHERE mac_address = \"$mac_address\"");
        $device = $result->fetch_assoc();
        $raw = "image_data/" . $mac_address . ".static";
        if ($device["device_type"] == 5) {
            $result = mysqli_query($mysqli, $sql_query);
            $raw = "image_data/" . $mac_address . ".static";
        } else {
            `./get_image.sh $mac_address 10 2>&1`;
            $raw = "image_data/" . $mac_address;
        }
        $file = "image_data/" . $mac_address . ".png";
        `./rawToPng.sh $raw 640 384`;
        if (file_exists($file)) {
            header('Content-Type: image/png');
            header("Cache-Control: no-store, no-cache, must-revalidate, max-age=0");
            header("Cache-Control: post-check=0, pre-check=0", false);
            header("Pragma: no-cache");
            header('Pragma: public');
            readfile($file);
            exit;
        }
    } else {
        echo "REGEX doesn't match";
    }
?>
