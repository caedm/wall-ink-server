<?php
function schedulingGetImage($config, $device, $schedule, $displayUrl, $qrCodeString) {
    $date=`date -d "+4 minutes" +%Y-%m-%d`;
    $time=`date -d "+4 minutes" +%H:%M`;
    $macAddressInfoFilePath = "image_data/" . $device["mac_address"] . ".info";
    $macAddressInfo = $device["mac_address"];
    $macAddressInfo .= "\n";
    $macAddressInfo .= $date;
    $macAddressInfo .= $time;
    $macAddressInfo .= $device["device_type"];
    $macAddressInfo .= "\n";
    $macAddressInfo .= $device["voltage"];
    $macAddressInfo .= "\n";
    $macAddressInfo .= $device["orientation"];
    $macAddressInfo .= "\n";
    $macAddressInfo .= $device["resource_id"];
    $macAddressInfo .= "\n";
    $macAddressInfo .= $displayUrl;
    $macAddressInfo .= "\n";
    $macAddressInfo .= $qrCodeString;
    $macAddressInfo .= "\n";
    $macAddressInfo .= $schedule;
    $macAddressInfoFile = fopen("$macAddressInfoFilePath", "w") or die("Unable to open file");
    fwrite($macAddressInfoFile, $macAddressInfo);
    `./genimg $macAddressInfoFilePath`;
    $imagePath = "./image_data/" . $device["mac_address"] . ".wink";
    return $imagePath;
}
?>
