<style>
<?php include 'css/edit_device.css'; ?>
</style>
<?php
error_reporting(E_ALL);
ini_set('display_errors', '1');
    $device_id = $_GET["device_id"];
    include '../config/dbconfig.php';
    if ($_GET["device_id"] == "new") {
        $device = array(
            "mac_address" => "",
            "resource_id" => "",
            "orientation" => 0,
            "device_type" => 0,
            "scheduling_system" => 0
        );
    } else if (preg_match('/^[[:digit:]]+$/', $device_id) === 1) {
        $mysqli = mysqli_connect($config->deviceDatabaseServer, $config->deviceDatabaseUsername, $config->deviceDatabasePassword, $config->deviceDatabaseName);
        $result = mysqli_query($mysqli, "SELECT * FROM devices WHERE device_id = $device_id");
        $device = mysqli_fetch_assoc($result);
    }
    $roomsArray = array();
    foreach (glob("../plugins/*.php") as $filename) {
        require_once($filename);
    }
    foreach ($plugins as $plugin) {
        $roomsArray[$plugin->getName()] = $plugin->getResources($config);
    }

    echo "<div>";
    echo "<form id=\"form\" action=\"handle_edit_device.php\" method=\"post\">";
        echo "<input type=\"hidden\" name=\"new_device_id\" value=\"$device_id\"/>";
        echo "<div class=\"field\">";
            echo "<label for=\"new_mac_address\">Mac Address:</label>";
            echo "<input type=\"text\" id=\"mac_address\" name=\"new_mac_address\" value=\"$device[mac_address]\">";
        echo "</div>";
        echo "<div class=\"field\">";
            echo "<label for=\"new_scheduling_system\">Scheduling System:</label>";
            echo "<select id=\"scheduling_system\" name=\"new_scheduling_system\">";
                foreach ($plugins as $plugin) {
                    echo "<option value=\"";
                    echo $plugin->getIndex();
                    echo "\"";
                    if ($device["scheduling_system"] == $plugin->getIndex()) {
                        echo " selected";
                    }
                    echo ">";
                    echo $plugin->getName();
                    echo "</option>";
                }
            echo "</select>";
        echo "</div>";
        foreach ($plugins as $plugin) {
            echo "<div class=\"field resource";
            if ($device["scheduling_system"] != $plugin->getIndex()) {
                echo " hidden";
            }
            echo "\">";
            echo "<label for=\"new_resource_id\">";
            echo $plugin->getName();
            echo " resource:</label>";
                echo "<select name=\"new_resource_id\">";
                foreach ($roomsArray[$plugin->getName()] as $key=>&$room) {
                    echo "<option value=\"";
                    echo $key;
                    echo "\"";
                    if ($key == $device["resource_id"]) {
                        echo " selected";
                    }
                    echo ">";
                    echo $room;
                    echo "</option>";
                }
                echo "</select>";
            echo "</div>";
        }
        echo "<fieldset class=\"field\">";
            echo "<legend>Device Type</legend>";
            echo "<ul>";
                echo "<li>";
                    echo "<label for=\"1\">4\" Landscape 1, 30 minute refresh cycle</label>";
                    echo "<input type=\"radio\" id=\"type_1\" name=\"new_device_type\" value=\"1\"";
                    if ($device['device_type'] == 1) {
                        echo " checked";
                    }
                    echo ">";
                echo "</li>";
                echo "<li>";
                    echo "<label for=\"4\">4\" Landscape 2, 30 minute refresh cycle</label>";
                    echo "<input type=\"radio\" id=\"type_4\" name=\"new_device_type\" value=\"4\"";
                    if ($device['device_type'] == 4) {
                        echo " checked";
                    }
                    echo ">";
                echo "</li>";
                echo "<li>";
                    echo "<label for=\"6\">4\" Landscape 3 (with QR code), 30 minute refresh cycle</label>";
                    echo "<input type=\"radio\" id=\"type_6\" name=\"new_device_type\" value=\"6\"";
                    if ($device['device_type'] == 6) {
                        echo " checked";
                    }
                    echo ">";
                echo "</li>";
                echo "<li>";
                    echo "<label for=\"9\">4\" Landscape 4 (with QR code), 30 minute refresh cycle</label>";
                    echo "<input type=\"radio\" id=\"type_9\" name=\"new_device_type\" value=\"9\"";
                    if ($device['device_type'] == 9) {
                        echo " checked";
                    }
                    echo ">";
                echo "</li>";
                echo "<li>";
                    echo "<label for=\"8\">4\" Static Image, 30 minute refresh cycle</label>";
                    echo "<input type=\"radio\" id=\"type_8\" name=\"new_device_type\" value=\"8\"";
                    if ($device['device_type'] == 8) {
                        echo " checked";
                    }
                    echo ">";
                echo "</li>";
                echo "<li>";
                    echo "<label for=\"0\">7\" Portrait, 15 minute refresh cycle</label>";
                    echo "<input type=\"radio\" id=\"type_0\" name=\"new_device_type\" value=\"0\"";
                    if ($device['device_type'] == 0) {
                        echo " checked";
                    }
                    echo ">";
                echo "</li>";
                echo "<li>";
                    echo "<label for=\"2\">7\" Landscape 1, 15 minute refresh cycle</label>";
                    echo "<input type=\"radio\" id=\"type_2\" name=\"new_device_type\" value=\"2\"";
                    if ($device['device_type'] == 2) {
                        echo " checked";
                    }
                    echo ">";
                echo "</li>";
                echo "<li>";
                    echo "<label for=\"3\">7\" Landscape 2, 30 minute refresh cycle</label>";
                    echo "<input type=\"radio\" id=\"type_3\" name=\"new_device_type\" value=\"3\"";
                    if ($device['device_type'] == 3) {
                        echo " checked";
                    }
                    echo ">";
                echo "</li>";
                echo "<li>";
                    echo "<label for=\"7\">7\" Landscape 3 (with QR code), 30 minute refresh cycle</label>";
                    echo "<input type=\"radio\" id=\"type_7\" name=\"new_device_type\" value=\"7\"";
                    if ($device['device_type'] == 7) {
                        echo " checked";
                    }
                    echo ">";
                echo "</li>";
                echo "<li>";
                    echo "<label for=\"10\">7\" Landscape 4 (with QR code), 30 minute refresh cycle, better battery</label>";
                    echo "<input type=\"radio\" id=\"type_10\" name=\"new_device_type\" value=\"10\"";
                    if ($device['device_type'] == 10) {
                        echo " checked";
                    }
                    echo ">";
                echo "</li>";
                echo "<li>";
                    echo "<label for=\"7\">7\" Static Image, 30 minute refresh cycle</label>";
                    echo "<input type=\"radio\" id=\"type_5\" name=\"new_device_type\" value=\"5\"";
                    if ($device['device_type'] == 5) {
                        echo " checked";
                    }
                    echo ">";
                echo "</li>";
                echo "<li>";
                    echo "<label for=\"7\">7\" landscape for event spaces</label>";
                    echo "<input type=\"radio\" id=\"type_11\" name=\"new_device_type\" value=\"11\"";
                    if ($device['device_type'] == 11) {
                        echo " checked";
                    }
                    echo ">";
                echo "</li>";
            echo "</ul>";
        echo "</fieldset>";
        echo "<fieldset id=\"orientation\" class=\"field\"";
        if ($device['device_type'] == 5 || $device['device_type'] == 8) {
            echo " style=\"display: none\"";
        }
        echo ">";
            echo "<legend>Display Orientation</legend>";
            echo "<ul>";
                echo "<li>";
                    echo "<label for=\"right-side_up\">Right-Side Up</label>";
                    echo "<input type=\"radio\" id=\"orientation_0\" name=\"new_orientation\" value=\"0\"";
                    if ($device['orientation'] == 0) {
                        echo " checked";
                    }
                    echo ">";
                echo "</li>";
                echo "<li>";
                    echo "<label for=\"up-side_down\">Up-Side Down</label>";
                    echo "<input type=\"radio\" id=\"orientation_1\" name=\"new_orientation\" value=\"1\"";
                    if ($device['orientation'] == 1) {
                        echo " checked";
                    }
                    echo ">";
                echo "</li>";
            echo "</ul>";
        echo "</fieldset>";
        echo "<div class=\"button\">";
            echo "<button type=\"button\" onclick=\"window.location.href='view_devices.php'\">Cancel</button>";
            echo "<button type=\"button\" class=\"middle\" onclick=\"deleteLists(); document.getElementById('form').submit()\">Submit</button>";
            if ($_GET["device_id"] != "new") {
                echo "<button type=\"button\" onclick=\"if (confirm('Are you sure you want to delete this device?') == true) window.location.href='handle_delete_device.php?device_id=$device[device_id]'\">Delete</button>";
            }
        echo "</div>";
    echo "</form>";

    echo "<form id=\"upload\" action=\"handle_upload.php\" method=\"post\" enctype=\"multipart/form-data\"";
    if ($device['device_type'] != 5 && $device['device_type'] != 8) {
        echo " style=\"display: none\"";
    }
    echo ">";
        echo "<input type=\"file\" name=\"staticImage\">";
        echo "<br>";
        echo "<input type=\"submit\" value=\"Upload\">";
        echo "<input type=\"hidden\" id=\"hidden_mac\" name=\"mac_address\" value=\"$device[mac_address]\">";
        echo "<input type=\"hidden\" id=\"hidden_device_id\" name=\"device_id\" value=\"$device[device_id]\">";
        echo "<input type=\"hidden\" id=\"hidden_device_type\" name=\"layout\" value=\"$device[device_type]\">";
    echo "</form>";
    echo "</div>";

    echo "<img id=\"preview\" src=../get_png.php?mac_address=$device[mac_address]&layout=$device[device_type] />";
    echo "<script src=\"js/edit_device.js\"></script>";
?>
