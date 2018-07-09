<?php
    #Debug stuff
    #require_once("print_info.php");
    #printInfo($_POST);

    #ini_set('display_errors', 1);
    #ini_set('display_startup_errors', 1);
    #error_reporting(E_ALL);

    $mac_address = $_GET["mac_address"];
    $png = "image_data/" . $mac_address . ".png";
    if (preg_match('/^[[:xdigit:]]+$/', $mac_address) === 1) {
        include 'config/dbconfig.php';
        $mysqli = mysqli_connect($deviceDatabaseServer, $deviceDatabaseUsername, $deviceDatabasePassword, "door-display");
        $result = mysqli_query($mysqli, "SELECT * FROM devices WHERE mac_address = \"$mac_address\"");
        $device = $result->fetch_assoc();
        if ($_GET["layout"] == 5 || $_GET["layout"] == 8) {
            #$result = mysqli_query($mysqli, $sql_query);
            $pbm = "image_data/" . $mac_address . ".static.pbm";
            `convert $pbm $png`;
        } else {
            `./get_image.sh $mac_address $device[voltage] 0 $_GET[layout] 2>&1`;
            $raw = "image_data/" . $mac_address;
            `./rawToPng.sh $raw $_GET[layout]`;
        }
        if (file_exists($png)) {
            header('Content-Type: image/png');
            header("Cache-Control: no-store, no-cache, must-revalidate, max-age=0");
            header("Cache-Control: post-check=0, pre-check=0", false);
            header("Pragma: no-cache");
            header('Pragma: public');
            readfile($png);
            exit;
        }
    } else {
        echo "REGEX doesn't match";
    }
?>
