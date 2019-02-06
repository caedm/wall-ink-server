<style>
<?php include 'css/view_logs.css'; ?>
</style>
<?php
    $mac_address = $_GET["mac_address"];
    $device_id = $_GET["device_id"];
    echo "<a href=\"view_devices.php\">Home</a>";
    echo "<br><br>";
    echo "<a href=\"edit_device.php?device_id=$device_id\">Back</a>";
    $logfile = fopen("../log/$mac_address.log", "r");
    $logs = fread($logfile, filesize("../log/$mac_address.log"));
    fclose($logfile);
    echo "<div><p>";
    echo nl2br($logs);
    echo "</p></div>";
?>

