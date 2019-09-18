<?php
require_once(dirname(__FILE__) . "/../plugin_dependencies/iPlugin.php");
require(dirname(__FILE__) . "/../config/dbconfig.php");

class exampleSchedulerPlugin implements iPlugin {

    private function getResourceName($resourceId) {
        //  This function would usually be internal to the scheduler's API
        //  We are querying the scheduler's resourceID looking for the common name of the resource

        //  We are statically defining an array of resources here since we are simulating having a database and an API
        $resources = array("1"=>"Conference Room 1","2"=>"Conference Room 2","3"=>"Banquet Room","4"=>"Patio","5"=>"Theater");
        $resourceName = $resources[$resourceId];
        return $resourceName;
    }


    public function getIndex() {
        // The number returned here for this plugin must be unique to this wall-ink-server
        return 3;
    } 
    public function getName() {
        // This is the name of the plugin as it will appear in the device manager
        return "Example Scheduler";
    } 
    public function isActive($config) {
        // Used to see if the plugin is active
        return $config->exampleSchedulerActive;
    }
    public function getResources($config) {
        // Build array of schedualable resources
        // The resourceID in the array do not need to be unique to each plugin, and may be any number or string VARCHAR(64)
        // Typically loading this array will be done by querying an API or a database to get a list of schedulable resources
        // This array will be used to fill the drop down list of resources in the device manager edit device page 
        $rooms = array("1"=>"Conference Room 1","2"=>"Conference Room 2","3"=>"Banquet Room","4"=>"Patio","5"=>"Theater");
        return $rooms;
    }
    private function getSchedule($config, $resourceId) {

        // To assist in debugging the getSchedule command, look in your web directory in the 
        // image_data folder.  Look for a file named <mac_address>.info which will contain the
        // text schedule for your resource.

        // Add the name of the resource to the top of the schedule
        $schedule = $this->getResourceName($resourceId);
        $schedule .="\n";

        // Usually this function would be getting today's event info from a database or API.
        // Since we are creating dates with today's date on them to display, we need to get today's
        // date and format properly for the image generator.	
        $formatted_date = date("Y-m-d");		

        // This function would typically use an API or other code to query some other resource
        // to get the a schedule and put it into the proper format.
        // Since this example does not have an online API or database to query, the schedules are
        // hard coded into this function

        switch($resourceId) {

        case "1": 
            $schedule .= "Board Meeting";
            $schedule .= "\n";
            $schedule .= $formatted_date . " 10:00:00";
            $schedule .= "\n";
            $schedule .= $formatted_date . " 12:00:00";
            $schedule .= "\n";
            break;
        case "2": 
            $schedule .= "Sales Meeting";
            $schedule .= "\n";
            $schedule .= $formatted_date . " 10:00:00";
            $schedule .= "\n";
            $schedule .= $formatted_date . " 11:00:00";
            $schedule .= "\n";
            $schedule .= "Marketing Meeting";
            $schedule .= "\n";
            $schedule .= $formatted_date . " 14:00:00";
            $schedule .= "\n";
            $schedule .= $formatted_date . " 15:00:00";
            $schedule .= "\n";
            break;
        case "3": 
            $schedule .= "Networking Luncheon";
            $schedule .= "\n";
            $schedule .= $formatted_date . " 11:00:00";
            $schedule .= "\n";
            $schedule .= $formatted_date . " 13:00:00";
            $schedule .= "\n";
            $schedule .= "Awards Ceremony";
            $schedule .= "\n";
            $schedule .= $formatted_date . " 16:00:00";
            $schedule .= "\n";
            $schedule .= $formatted_date . " 17:00:00";
            $schedule .= "\n";
            break;
        case "4": 
            $schedule .= "Vendor Barbeque";
            $schedule .= "\n";
            $schedule .= $formatted_date . " 11:00:00";
            $schedule .= "\n";
            $schedule .= $formatted_date . " 13:00:00";
            $schedule .= "\n";
            break;
        case "5": 
            $schedule .= "Secret Project Screening";
            $schedule .= "\n";
            $schedule .= $formatted_date . " 14:00:00";
            $schedule .= "\n";
            $schedule .= $formatted_date . " 15:00:00";
            $schedule .= "\n";
            $schedule .= "Dave's Movie Night";
            $schedule .= "\n";
            $schedule .= $formatted_date . " 20:00:00";
            $schedule .= "\n";
            $schedule .= $formatted_date . " 22:00:00";
            $schedule .= "\n";
            break;
        default:
            $schedule .= "Error: resource " . $resourceId . " not found";
            $schedule .= "\n";
        } 
        return $schedule;

    }
    public function getImage($config, $device) {
        require_once(dirname(__FILE__) . "/../plugin_dependencies/general_scheduling/schedulingGetImage.php");
        return schedulingGetImage($config, $device, $this->getSchedule($config, $device["resource_id"]), $config->exampleSchedulerDisplayUrl, $config->exampleSchedulerQrCodeBaseUrlBeginning . $device["resource_id"] . $config->exampleSchedulerQrCodeBaseUrlEnd);
    }
    public function getDeviceType($device) {
        require_once(dirname(__FILE__) . "/../plugin_dependencies/general_scheduling/schedulingGetDeviceType.php");
        return schedulingGetDeviceType($device, $this->getIndex());
    }
}

if ($config->exampleSchedulerActive == "true") {
    $exampleScheduler = new exampleSchedulerPlugin;
    $plugins[ $exampleScheduler->getIndex() ] = $exampleScheduler;
}
?>
