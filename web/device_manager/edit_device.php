<style>
<?php include 'css/edit_device.css'; ?>
</style>
<?php
    $device_id = $_GET["device_id"];
    if ($_GET["device_id"] == "new") {
        $device = array(
            "mac_address" => "",
            "resource_id" => "",
            "orientation" => 0,
            "device_type" => 0
        );
    } else if (preg_match('/^[[:digit:]]+$/', $device_id) === 1) {
        include 'dbconfig.php';
        $mysqli = mysqli_connect($server, $username, $password, "door-display");
        $result = mysqli_query($mysqli, "SELECT * FROM devices WHERE device_id = $device_id");
        $device = mysqli_fetch_assoc($result);
    }
    $mysqli = mysqli_connect($server, $username, $password, "collegeresv");
    $resources = mysqli_query($mysqli, "SELECT resource_id,name FROM resources");
    $rooms = array();
    while($room = $resources->fetch_assoc()){
       $rooms[ $room["resource_id"] ] = $room;
    }

    echo "<form action=\"/device_manager/handle_edit_device.php\" method=\"post\">";
        echo "<input type=\"hidden\" name=\"new_device_id\" value=\"$device_id\"/>";
        echo "<div class=\"field\">";
            echo "<label for=\"new_mac_address\">Mac Address:</label>";
            echo "<input type=\"text\" id=\"mac_address\" name=\"new_mac_address\" value=\"$device[mac_address]\">";
        echo "</div>";
        echo "<div class=\"field\">";
            echo "<label for=\"new_resource_id\">Room:</label>";
            echo "<select id=\"resource_id\" name=\"new_resource_id\">";
            foreach ($rooms as &$room) {
                echo "<option value=\"$room[resource_id]\"";
                if ($room["resource_id"] == $device["resource_id"]) {
                    echo " selected";
                }
                echo ">";
                echo "$room[name]</option>";
            }
            echo "</select>";
        echo "</div>";
        echo "<fieldset class=\"field\">";
            echo "<legend>Device Type</legend>";
            echo "<ul>";
                echo "<li>";
                    echo "<label for=\"0\">7\" Portrait</label>";
                    echo "<input type=\"radio\" id=\"type_0\" name=\"new_device_type\" value=\"0\"";
                    if ($device['device_type'] == 0) {
                        echo " checked";
                    }
                    echo ">";
                echo "</li>";
                echo "<li>";
                    echo "<label for=\"1\">4\" Landscape</label>";
                    echo "<input type=\"radio\" id=\"type_1\" name=\"new_device_type\" value=\"1\"";
                    if ($device['device_type'] == 1) {
                        echo " checked";
                    }
                    echo ">";
                echo "</li>";
                echo "<li>";
                    echo "<label for=\"2\">7\" Landscape 1</label>";
                    echo "<input type=\"radio\" id=\"type_2\" name=\"new_device_type\" value=\"2\"";
                    if ($device['device_type'] == 2) {
                        echo " checked";
                    }
                    echo ">";
                echo "</li>";
                echo "<li>";
                    echo "<label for=\"3\">7\" Landscape 2</label>";
                    echo "<input type=\"radio\" id=\"type_3\" name=\"new_device_type\" value=\"3\"";
                    if ($device['device_type'] == 3) {
                        echo " checked";
                    }
                    echo ">";
                echo "</li>";
            echo "</ul>";
        echo "</fieldset>";
        echo "<fieldset class=\"field\">";
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
            echo "<button type=\"submit\">Update Device Settings</button>";
            echo "<button type=\"button\" onclick=\"if (confirm('Are you sure you want to delete this device?') == true) window.location.href='/device_manager/handle_delete_device.php?device_id=$device[device_id]'\">Delete Device</button>";
        echo "</div>";
    echo "</form>";
?>
