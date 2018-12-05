<?php

function getResourceConfig($config,$device) {
	$simpleTextConfig = new StdClass();

	$simpleTextConfig->font="DejaVu-Sans";
	$simpleTextConfig->margin="25";
	$simpleTextConfig->pointsize="14";


	$display_text =  "Wall Ink Simple Text Plugin\n";

	$display_text .= "\n";
	$display_text .= "Device Settings";
	$display_text .= "\n";
	
	$display_text .= "MAC Address: " . $device['mac_address'];
	$display_text .= "\n";
	
	$display_text .="Resource ID: " . $device['resource_id'];
	$display_text .= "\n";

	$display_text .="Device Type: " . $device['device_type'];
	$display_text .= "\n";
	
	$display_text .="Voltage: " . $device['voltage'];
	$display_text .= "\n";

	$display_text .="Firmware Version: " . $device['firmware_version'];
	$display_text .= "\n";

	$display_text .="Plugin: " . $device['plugin'];
	$display_text .= "\n";

	$display_text .= "\n";
	$display_text .="Server Configuration";
	$display_text .= "\n";

	$display_text .="Debug Mode Active: " . $config->debugModeActive;
	$display_text .= "\n";

	$display_text .="Server URL " . $config->runTimeWebDirectory;
	$display_text .= "\n";

	$display_text .="Database Server: " . $config->deviceDatabaseServer;
	$display_text .=" Database Name: " . $config->deviceDatabaseName;
	$display_text .= "\n";

	$simpleTextConfig->caption=$display_text;

	return $simpleTextConfig;
}
