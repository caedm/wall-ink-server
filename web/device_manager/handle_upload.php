<?php

error_reporting(E_ALL);
ini_set('display_errors', '1');

define("UPLOAD_DIR","../image_data/");

$success = FALSE;

if (!empty($_FILES["staticImage"])) {
    $staticImage = $_FILES["staticImage"];

    if ($staticImage["error"] !== UPLOAD_ERR_OK) {
        echo "<p>Upload error</p>";
        exit;
    }

    $macAddress = UPLOAD_DIR . $_POST["mac_address"];
    $pbm = $macAddress . ".static.pbm";
    $png = $macAddress . ".png";
    $static = $macAddress . ".static";

    //preserve file from temp directory
    $success = move_uploaded_file($staticImage["tmp_name"], $pbm);
    if (!$success) {
        echo "<p>File save error</p>";
        exit;
    }

    //set permissions on the new image
    chmod($pbm, 0664);
    
    //convert image from pbm to png for device manager
    `convert $pbm $png`;
    
    //convert file from pbm to compressed for display screens
    `../pbmToCompressed $pbm $static`;
}

if ($success) {
    header( "refresh: 1; url=edit_device.php?device_id=$_POST[device_id]");
    echo "Image uploaded successfully";
} else {
    echo "Error uploading image";
}

?>
