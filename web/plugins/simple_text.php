<?php
require_once("$_SERVER[DOCUMENT_ROOT]/plugin_dependencies/iPlugin.php");
require("$_SERVER[DOCUMENT_ROOT]/config/dbconfig.php");

class simpleTextPlugin implements iPlugin {

    public function getIndex() {
        // The number returned here for this plugin must be unique to this wall-ink-server
        return 4;
    } 
    public function getName() {
        // This is the name of the plugin as it will appear in the device manager
        return "Simple Text";
    } 
    public function isActive($config) {
        // Used to see if the plugin is active
        return $config->simpleTextPluginActive;
    }
    public function getResources($config) {
	//Get a list of all of the php files in the magick folder and 
	//use that to fill the resources menu in the device manager.
	$magickFiles = array();
	//get the list of all php files in the magick folder
	$filelist = glob($config->runTimeWebDirectory . '/plugin_dependencies/simple_text' . '/*' . '.php' , GLOB_BRACE);
	 foreach ($filelist as $filename) {
		//get rid of the path in front of the file name, and strip the last 4 characters off the name
            	$magickFiles[ substr(substr($filename, strlen($filename) - strpos(strrev($filename), '/')),0,-4) ] = substr(substr($filename, strlen($filename) - strpos(strrev($filename), '/')),0,-4);
        	}
	return $magickFiles;
    }
    public function getImage($config, $device) {

	$resource_file = "$_SERVER[DOCUMENT_ROOT]/plugin_dependencies/simple_text/" . $device['resource_id'] . ".php";	
	
	// if the device has a resource php file, and it exists, then load it.
	if (file_exists($resource_file)) {
		require_once($resource_file);	
		//get settings and messages from the resource	
		$my_simpleTextConfig = getResourceConfig($config,$device);	
		
		//margin size around text so it isn't right against the edge of the screen
		$font = $my_simpleTextConfig->font;
		$margin_size = $my_simpleTextConfig->margin;	
		$pointsize = $my_simpleTextConfig->pointsize;
		$caption = $my_simpleTextConfig->caption;
		
	} else {
	// if the device resource php file is not set or does not exist
	// then use some default text settings to pass back that the resource is not there
			
		//margin size around text so it isn't right against the edge of the screen
		$margin_size = "15";
		$pointsize = "12";
		$caption = "Unable to load " . $resource_file;	
		$font = "DejaVu-Sans";
	}
	
	//get height and width of image from wall-ink device firmware
	$width = $device['width'];
	$height = $device['height'];

	//The size of the image created by ImageMagick below must be reduced by the size of the margin added by the "border"
	$size = ($width - $margin_size - $margin_size) . "x" . ($height - $margin_size - $margin_size);
	
	//if device is set to upside-down, make it so
        $angle = 0;
        if ($device['orientation'] == 1) {
                $angle = 180;
        }
	
	//use device type as the number of minutes to sleep for.  If device type not set, assume 1 hour
	$timeIncrement = 3600;
	if (isset($device['device_type'])) {
		$timeIncrement = $device['device_type'] * 60;
	}		
	
	//calculate the next time the wall-ink device is to check in
 	$nextRefreshTime = $timeIncrement - ($_SERVER['REQUEST_TIME'] % $timeIncrement) +30;
        $pbm = "$_SERVER[DOCUMENT_ROOT]/image_data/" . $device["mac_address"] . "." . "pbm";
        $raw = "$_SERVER[DOCUMENT_ROOT]/image_data/" . $device["mac_address"];
        $textImage = "$_SERVER[DOCUMENT_ROOT]/image_data/" . $device["mac_address"] . "." . "wink";

	
	`convert -background white -fill black -font $font -border $margin_size -bordercolor white -size $size -pointsize $pointsize -rotate $angle caption:"$caption" $pbm`;
	`$_SERVER[DOCUMENT_ROOT]/pbmToRaw.sh $pbm $raw`;
	`$_SERVER[DOCUMENT_ROOT]/rawToWink $raw $textImage $width $height $nextRefreshTime $device[mac_address]`;

        return $textImage;
    }

    public function getDeviceType($device) {
	//There are no device type options for this plugin, it handles it automagickly
  $getDeviceType = "";
        $getDeviceType .= "<fieldset class=\"field getdevicetype";
        if ($device['plugin'] != $this->getIndex()) {
            $getDeviceType .= " hidden";
        }
        $getDeviceType .= "\" data-pluginid=\"";
        $getDeviceType .= $this->getIndex();
        $getDeviceType .= "\">";
            $getDeviceType .= "<legend>Device Type</legend>";
            $getDeviceType .= "<ul>";
                $getDeviceType .= "<li>";
                    $getDeviceType .= "<label for=\"30\">30 minute refresh cycle</label>";
                    $getDeviceType .= "<input type=\"radio\" id=\"type_30\" name=\"new_device_type\" value=\"30\"";
                    if ($device['device_type'] == 30 && $device['plugin'] == $this->getIndex()) {
                        $getDeviceType .= " checked";
                    }
                    $getDeviceType .= ">";
                $getDeviceType .= "</li>";
                $getDeviceType .= "<li>";
                    $getDeviceType .= "<label for=\"60\">1 hour refresh cycle</label>";
                    $getDeviceType .= "<input type=\"radio\" id=\"type_60\" name=\"new_device_type\" value=\"60\"";
		    if ($device['device_type'] == 60 && $device['plugin'] == $this->getIndex()) {
                        $getDeviceType .= " checked";
                    }
                    $getDeviceType .= ">";
                $getDeviceType .= "</li>";
            $getDeviceType .= "</ul>";
        $getDeviceType .= "</fieldset>";
        return $getDeviceType;

    }


}

if ($config->simpleTextPluginActive == "true") {
    $simpleText= new simpleTextPlugin;
    $plugins[ $simpleText->getIndex() ] = $simpleText;
}
?>
