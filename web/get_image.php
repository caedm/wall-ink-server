<?php
    #Debug stuff
    #require_once("print_info.php");
    #printInfo($_POST);

    $mac_address = $_GET["mac_address"];
    $voltage = $_GET["voltage"];
    if (preg_match('/^[[:xdigit:]]+$/', $mac_address) === 1 && preg_match('/^[[:digit:].]+$/', $voltage) === 1) {
        include 'device_manager/dbconfig.php';
        $mysqli = mysqli_connect($server, $username, $password, "door-display");
        $result = mysqli_query($mysqli, "SELECT * FROM devices WHERE mac_address = \"$mac_address\"");
        $device = $result->fetch_assoc();
        $file = "image_data/" . $mac_address . ".static";
        if ($device["device_type"] == 5) {
            if ($device["voltage"] + 0.35 < "$voltage"){
                $sql_query="UPDATE devices SET batteries_replaced_date = NOW() WHERE mac_address = \"$mac_address\"";
                $result = mysqli_query($mysqli, $sql_query);
            }
            $sql_query="UPDATE devices SET voltage = $voltage, last_checked_in = NOW() WHERE mac_address = \"$mac_address\"";
            $result = mysqli_query($mysqli, $sql_query);
            $file = "image_data/" . $mac_address . ".static";
        } else {
            `./get_image.sh $mac_address $voltage 2>&1`;
            $file = "image_data/" . $mac_address . ".compressed";
        }
        if (file_exists($file)) {
            header('Content-Description: File Transfer');
            header('Content-Type: application/octet-stream');
            header('Content-Disposition: attachment; filename="'.basename($file).'"');
            header('Expires: 0');
            header("Cache-Control: no-store, no-cache, must-revalidate, max-age=0");
            header("Cache-Control: post-check=0, pre-check=0", false);
            header("Pragma: no-cache");
            header('Pragma: public');
            header('Content-Length: ' . filesize($file));
            readfile($file);
            exit;
        }
    } else {
        echo "REGEX doesn't match";
    }
?>
