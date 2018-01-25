<?php
    $mac_address = $_GET["mac_address"];
    $voltage = $_GET["voltage"];
    if (preg_match('/^[[:xdigit:]]+$/', $mac_address) === 1 && preg_match('/^[[:digit:].]+$/', $voltage) === 1) {
        `./get_image.sh $mac_address $voltage 2>&1`;
        $file = "image_data/" . $mac_address . ".compressed";
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
    }
?>
