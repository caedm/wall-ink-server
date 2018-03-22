<?php

error_reporting(E_ALL);
ini_set('display_errors', '1');

define("UPLOAD_DIR","../image_data/");

$success = FALSE;
$error = FALSE;

if (!empty($_FILES["staticImage"])) {
    $staticImage = $_FILES["staticImage"];

    exec("file -bi " . $_FILES["staticImage"]["tmp_name"], $out);
    if ($out[0] != "image/x-portable-bitmap; charset=binary") {
        echo "<p>Image must be a binary .pbm file</p>";
        $error = TRUE;
        exit;
    }

    if ($staticImage["error"] !== UPLOAD_ERR_OK) {
        echo "<p>Upload error</p>";
        $error = TRUE;
        exit;
    }

    $macAddress = UPLOAD_DIR . $_POST["mac_address"];
    $pbm = $macAddress . ".static.pbm";
    $png = $macAddress . ".png";
    $pbmBackup = $macAddress . ".static.pbmbackup";
    $pngBackup = $macAddress . ".pngbackup";
    $static = $macAddress . ".static";

    //make backup of old image
    rename($pbm, $pbmBackup);
    rename($png, $pngBackup);

    //preserve file from temp directory
    $success = move_uploaded_file($staticImage["tmp_name"], $pbm);
    if (!$success) {
        echo "<p>File save error</p>";
        $error = TRUE;
        exit;
    }

    //set permissions on the new image
    chmod($pbm, 0660);
    
    //convert image from pbm to png for device manager
    `convert $pbm $png`;

    $layout = $_POST["layout"];

    list($width, $height) = getimagesize($png);
    if (!(($width==640 && $height==384 && $layout==5) || ($width==400 && $height==300 && $layout==8))) {
        echo "<p>Image must be 640x384 for 7\" screen or 400x300 for 4\" screen</p>";
        $error = TRUE;
        rename($pbmBackup, $pbm);
        rename($pngBackup, $png);
        exit;
    }
    
    //convert file from pbm to compressed for display screens
    `../pbmToCompressed $pbm $static`;

}

if ($success && !$error) {
    header( "refresh: 1; url=edit_device.php?device_id=$_POST[device_id]");
    echo "Image uploaded successfully";
} else {
    echo "Error uploading image";
}

?>
