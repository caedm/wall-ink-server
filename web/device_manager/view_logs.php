<style>
<?php include 'css/view_logs.css'; ?>
</style>
<?php
    $mac_address = $_GET["mac_address"];
    $device_id = $_GET["device_id"];
    echo '<div class="main">';
    echo '<div class="sidebar">';
    echo "<a href=\"view_devices.php\">Home</a>";
    echo "<br>";
    echo "<a href=\"edit_device.php?device_id=$device_id\">Back</a>";
    echo "<div>";
    echo '<input type="checkbox" name="filterForErrors" id="filterForErrors" value="false"><label for="filterForErrors">Show only log entries with errors</label>';
    echo "</div>";
    echo '<div class="search">';
    echo 'Search: <input type="text" id="search" name="search">';
    echo "</div>";
    echo "</div>";
    $logfile = fopen("../log/$mac_address.log", "r");
    $logs = fread($logfile, filesize("../log/$mac_address.log"));
    fclose($logfile);
    $logsArray = explode("\n",$logs);
    echo "<div class=\"logs\">";
    foreach ($logsArray as $log) {
        echo "<p class=\"log\">";
        echo "$log";
        echo "</p>";
    }
    echo "</div>";
    echo "</div>";
?>
<script src="js/view_logs.js"></script>
