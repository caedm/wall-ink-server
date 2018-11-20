<?php

function getResourceConfig($config,$device) {
	$simpleTextConfig = new StdClass();

	$simpleTextConfig->font="DejaVu-Sans";
	$simpleTextConfig->margin="25";
	$simpleTextConfig->pointsize="14";

	//Display an error message as passed on. If no error, state such.
		
	if (isset($device['error_message'])) {
                $display_error = $device['error_message'];
        } else {
                $display_error = "No errors present";
        }

	$display_text =  "+++++++++++++++++++++++++++++++++++++++++++++\n";
        $display_text .= "                         Server Error Message\n";
        $display_text .= "+++++++++++++++++++++++++++++++++++++++++++++\n";
	$display_text .=$display_error . "\n";
	
	$display_text .= "\n";
	$display_text .= "Local Wall-ink Device Settings";
	$display_text .= "\n===============================\n";
	
	$display_text .= "Device MAC Address: " . $device['mac_address'];
	$display_text .= "\n";
	
	$display_text .="Device Voltage: " . $device['voltage'];
	$display_text .= "\n";

	$display_text .="Device Firmware Version: " . $device['firmware_version'];
	$display_text .= "\n";

	$display_text .= "\n";
	$display_text .="Remote Server Configuration";
	$display_text .= "\n===============================\n";

	$display_text .="Server Debug Mode Active: " . $config->debugModeActive;
	$display_text .= "\n";

	$display_text .="Server Directory " . $config->runTimeWebDirectory;
	$display_text .= "\n";

	$display_text .="Database Server and Name: " . $config->deviceDatabaseServer . " , " . $config->deviceDatabaseName;
	$display_text .= "\n";

	//text to return to ImageMagick to be displayed in pbm file
	$simpleTextConfig->caption=$display_text;

	return $simpleTextConfig;
}
