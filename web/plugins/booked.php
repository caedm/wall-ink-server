<?php
require_once(dirname(__FILE__) . "/../plugin_dependencies/iPlugin.php");
require(dirname(__FILE__) . "/../config/dbconfig.php");
class bookedPlugin implements iPlugin {
    public function getIndex() {
        return 0;
    }
    public function getName() {
        return "Booked";
    }
    public function isActive($config) {
        return $config->bookedIntegrationActive;
    }
    public function getResources($config) {
        $rooms = array();
        $mysqli = mysqli_connect(
            $config->bookedDatabaseServer,
            $config->bookedDatabaseUsername,
            $config->bookedDatabasePassword,
            $config->bookedDatabaseName
        );
        $resources = mysqli_query($mysqli, "SELECT resource_id,name FROM resources");
        while($room = $resources->fetch_assoc()){
            $rooms[ $room["resource_id"] ] = $room["name"];
        }
        return $rooms;
    }
    private function getSchedule($config, $resourceId) {
        $mysqli = mysqli_connect(
            $config->bookedDatabaseServer,
            $config->bookedDatabaseUsername,
            $config->bookedDatabasePassword,
            $config->bookedDatabaseName
        );
        $name = mysqli_query($mysqli, "SELECT name FROM resources WHERE resource_id = $resourceId");
        $schedule = "";
        while($row = $name->fetch_assoc()){
            $schedule = $row['name'];
        }
        $schedule .= "\n";
        $query = "SELECT
                    name,
                    title,
                    CONVERT_TZ(start_date, '+0:00', 'SYSTEM') AS start_date,
                    CONVERT_TZ(end_date,   '+0:00', 'SYSTEM') AS end_date
                  FROM
                    resources t1
                    JOIN reservation_resources t2 ON (t1.resource_id = t2.resource_id)
                    JOIN reservation_series    t3 ON (t2.series_id = t3.series_id)
                    JOIN reservation_instances t4 ON (t3.series_id = t4.series_id)
                  WHERE
                    t1.resource_id = $resourceId
                    AND DATE(CONVERT_TZ(start_date, '+0:00', 'SYSTEM')) <= CURRENT_DATE()
                    AND DATE(CONVERT_TZ(end_date,   '+0:00', 'SYSTEM')) >= CURRENT_DATE()
                    AND t3.status_id != 2
                  ORDER BY start_date
                 ";

        $reservations = mysqli_query($mysqli, $query);
        while ($reservation = $reservations->fetch_assoc()) {
            $schedule .= $reservation['title']      . "\n"
                       . $reservation['start_date'] . "\n"
                       . $reservation['end_date']   . "\n";
        }

        return $schedule;
    }
    public function getImage($config, $device) {
        require_once(dirname(__FILE__) . "/../plugin_dependencies/general_scheduling/schedulingGetImage.php");
        return schedulingGetImage(
            $config,
            $device,
            $this->getSchedule($config, $device["resource_id"]),
            $config->bookedDisplayUrl,
            $config->bookedQrCodeBaseUrlBeginning . $device["resource_id"] . $config->bookedQrCodeBaseUrlEnd);
    }
    public function getDeviceType($device) {
        require_once(dirname(__FILE__) . "/../plugin_dependencies/general_scheduling/schedulingGetDeviceType.php");
        return schedulingGetDeviceType($device, $this->getIndex());
    }
}
if ($config->bookedIntegrationActive == "true") {
    $booked = new bookedPlugin;
    $plugins[ $booked->getIndex() ] = $booked;
}
?>
