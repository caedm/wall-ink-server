<?php
require_once("$_SERVER[DOCUMENT_ROOT]/plugin_dependencies/iPlugin.php");
require("$_SERVER[DOCUMENT_ROOT]/config/dbconfig.php");
class googlePlugin implements iPlugin {
    public function getIndex() {
        return 1;
    } 
    public function getName() {
        return "Google Calendar";
    } 
    public function isActive($config) {
        return $config->googleIntegrationActive;
    }
    public function getResources($config) {
        require_once("$config->runTimeWebDirectory/plugin_dependencies/google/google_includes.php");

        // Get the API client and construct the service object.
        $client = getClient();
        $service = new Google_Service_Calendar($client);
        $calendarList = $service->calendarList->listCalendarList();

        // Build array of rooms 
        $rooms = array();
        foreach ($calendarList->getItems() as $calendarListEntry) {
            $rooms[$calendarListEntry->getId()] = $calendarListEntry->getSummary();
        }
        return $rooms;
    }
    private function getSchedule($config, $resourceId) {
        require_once("$_SERVER[DOCUMENT_ROOT]/plugin_dependencies/google/google_includes.php");

        // Time zone must be set manually, google api complains if set by variable for some reason
        date_default_timezone_set('America/Denver');
        // Get the API client and construct the service object.

        $client = getClient();
        $service = new Google_Service_Calendar($client);

        $calendar= $service->calendars->get($resourceId);
        $schedule=$calendar->getSummary();
        $schedule .="\n";

        $date_today = date('Y-m-d');
        $begin_today = $date_today . 'T00:00:00-07:00'; //make sure to modify for local time zone
        $end_today = $date_today . 'T23:59:59-07:00';  //make sure to modify for local time zone

        $optParams = array('orderBy' => 'startTime','singleEvents' => 'true','timeMax' => $end_today,'timeMin' => $begin_today,'showDeleted' => 'false');
        $events = $service->events->listEvents($resourceId,$optParams);

        foreach ($events->getItems() as $event) {
            $schedule .= $event->getSummary();
            $schedule .= "\n";
            $schedule .= $event->getStart()->dateTime;
            $schedule .= "\n";
            $schedule .= $event->getEnd()->dateTime;
            $schedule .= "\n";
        }
        return $schedule;
    }
    public function getImage($config, $device) {
        require_once("$_SERVER[DOCUMENT_ROOT]/plugin_dependencies/general_scheduling/schedulingGetImage.php");
        return schedulingGetImage($config, $device, $this->getSchedule($config, $device["resource_id"]), $config->googleCalendarDisplayUrl, $config->googleCalendarQrCodeBaseUrlBeginning, $config->googleCalendarQrCodeBaseUrlEnd);
    }
    public function getDeviceType($device) {
        require_once("$_SERVER[DOCUMENT_ROOT]/plugin_dependencies/general_scheduling/schedulingGetDeviceType.php");
        return schedulingGetDeviceType($device, $this->getIndex());
    }
}
if ($config->googleIntegrationActive == "true") {
    $google = new googlePlugin;
    $plugins[ $google->getIndex() ] = $google;
}
?>
