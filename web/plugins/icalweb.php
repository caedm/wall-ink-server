<?php
require_once("$_SERVER[DOCUMENT_ROOT]/plugin_dependencies/iPlugin.php");
require("$_SERVER[DOCUMENT_ROOT]/config/dbconfig.php");

class icalWebPlugin implements iPlugin {

    public function getIndex() {
        // The number returned here for this plugin must be unique to this wall-ink-server
        return 7;
    } 
    public function getName() {
        // This is the name of the plugin as it will appear in the device manager
        return "iCalWeb";
    } 
    public function isActive($config) {
        // Used to see if the plugin is active
        return $config->icalWebActive;
    }
    public function getResources($config) {
        require("$_SERVER[DOCUMENT_ROOT]/config/icalweb_calendars.php");
        $rooms = array();
        foreach ($icalweb_calendars as $icalweb_calendar){
            $rooms[$icalweb_calendar['id']] = $icalweb_calendar['resource'];
        }
        return $rooms;
    }
    private function getSchedule($config, $resourceId) {

        // To assist in debugging the getSchedule command, look in your web directory in the 
        // image_data folder.  Look for a file named <mac_address>.info which will contain the
        // text schedule for your resource.

        require_once("$_SERVER[DOCUMENT_ROOT]/plugin_dependencies/icalweb/icalparser/src/IcalParser.php");
        require_once("$_SERVER[DOCUMENT_ROOT]/plugin_dependencies/icalweb/icalparser/src/Recurrence.php");
        require_once("$_SERVER[DOCUMENT_ROOT]/plugin_dependencies/icalweb/icalparser/src/WindowsTimezones.php");
        require_once("$_SERVER[DOCUMENT_ROOT]/plugin_dependencies/icalweb/icalparser/src/Freq.php");

        require("$_SERVER[DOCUMENT_ROOT]/config/icalweb_calendars.php");

        $cal = new \om\IcalParser();
        $parse_url = $icalweb_calendars[$resourceId]['URL'];
        $results = $cal->parseFile($parse_url);

        // Get calendar name in this order of priority:  
        // 1. Manual entry "title" in config file
        // 2. X-WR-CALNAME from calendar
        // 3. If both of the above are blank, use the resource name from the config file
        if ((array_key_exists('title',$icalweb_calendars[$resourceId]) and $icalweb_calendars[$resourceId]['title'] != '')) {
            $calendar_title = $icalweb_calendars[$resourceId]['title'];
        } else if ( $cal->getiCalName() != '') {
            $calendar_title = $cal->getiCalName();
        } else {
            $calendar_title = $icalweb_calendars[$resourceId]['resource'];
        }

        $schedule = $calendar_title; 
        $schedule .= "\n";

        foreach ($cal->getSortedEvents() as $r) {
            $start_time = $r['DTSTART'];
            $end_time = $r['DTEND'];
		//force all events from whatever timezone they are to the local time of the ical calendar
		//the displays only show a time, and not a time zone, so force all times to local time
            date_timezone_set($start_time, timezone_open($cal->timezone->getname()));
            date_timezone_set($end_time, timezone_open($cal->timezone->getname()));
            if ($start_time->format('Y-m-d') == date("Y-m-d")){
                //if event is private, do not display name of event on screen
                if ((array_key_exists('CLASS',$r) and ($r['CLASS'] == 'PRIVATE')) ){
                    $schedule .= 'Reserved';
                } else {
                    $schedule .= $r['SUMMARY'];
                }
                $schedule .= "\n";
                $schedule .= $start_time->format('Y-m-d H:i:s');
                $schedule .= "\n";
                $schedule .= $end_time->format('Y-m-d H:i:s');
                $schedule .= "\n";
            }
        }
        return $schedule;
    }
    public function getImage($config, $device) {
        require_once("$_SERVER[DOCUMENT_ROOT]/plugin_dependencies/general_scheduling/schedulingGetImage.php");
        require("$_SERVER[DOCUMENT_ROOT]/config/icalweb_calendars.php");
        if ($config->icalWebQrCodeBaseUrlBeginning !== "") {
            $qrCodeString = $config->icalWebQrCodeBaseUrlBeginning . $device["resource_id"] . $config->icalWebQrCodeBaseUrlEnd;
        } else {
            $qrCodeString = "";
        }
        return schedulingGetImage(
            $config, 
            $device, 
            $this->getSchedule($config, $device["resource_id"]),
            $icalweb_calendars[$device["resource_id"]]['DisplayURL'], 
            $qrCodeString);
    }
    public function getDeviceType($device) {
        require_once("$_SERVER[DOCUMENT_ROOT]/plugin_dependencies/general_scheduling/schedulingGetDeviceType.php");
        return schedulingGetDeviceType($device, $this->getIndex());
    }
}

if ($config->icalWebPluginActive == "true") {
    $icalWeb = new icalWebPlugin;
    $plugins[ $icalWeb->getIndex() ] = $icalWeb;
}
?>
