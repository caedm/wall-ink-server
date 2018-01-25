<?php
    $device_id = $_GET["device_id"];
    if (preg_match('/^[[:digit:]]+$/', $device_id) === 1) {
        include 'dbconfig.php';
        $mysqli = mysqli_connect($server, $username, $password, "door-display");
        $result = mysqli_query($mysqli, "SELECT * FROM devices WHERE device_id = $device_id");
        $device = mysqli_fetch_assoc($result);

        echo "<form action=\"/handle_edit_device.php\" method=\"post\">";
            echo "<div>";
                echo "<label for=\"mac_address\">Mac Address:</label>";
                echo "<input type=\"text\" id=\"mac_address\" mac_address=\"Mac Address\">";
            echo "</div>";
            echo "<div>";
                echo "<label for=\"resource_id\">Resource ID:</label>";
                echo "<input type=\"text\" id=\"resource_id\" name=\"resource_id\">";
            echo "</div>";
            echo "<div>";
                echo "<label for=\"device_type\">Device Type:</label>";
                echo "<select id=\"device_type\" name=\"device_type\">";
                    echo "<option>1</option>";
                    echo "<option>2</option>";
                    echo "<option>3</option>";
                    echo "<option>4</option>";
                echo "</select>";
            echo "</div>";
            echo "<fieldset>";
                echo "<legend>Display Orientation</legend>";
                echo "<ul>";
                    echo "<li>";
                        echo "<label for=\"right-side_up\">Right-Side Up</label>";
                        echo "<input type=\"radio\" checked id=\"right-side_up\" name=\"orientation\" value=\"right-side_up\">";
                    echo "</li>";
                    echo "<li>";
                        echo "<label for=\"up-side_down\">Up-Side Down</label>";
                        echo "<input type=\"radio\" id=\"up-side_down\" name=\"orientation\" value=\"up-side_down\">";
                    echo "</li>";
                echo "</ul>";
            echo "</fieldset>";
            echo "<div class=\"button\">";
                echo "<button type=\"submit\">Submit Changes</button>";
            echo "</div>";
        echo "</form>";
    }
?>
