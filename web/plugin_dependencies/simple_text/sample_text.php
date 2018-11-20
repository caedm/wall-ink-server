<?php

function getResourceConfig($config,$device) {
	$simpleTextConfig = new StdClass();

	$simpleTextConfig->font="DejaVu-Sans";
	$simpleTextConfig->margin="25";
	$simpleTextConfig->pointsize="24";


	$display_text =  "Wall Ink Simple Text Plugin\n";

	$simpleTextConfig->caption=$display_text;

	return $simpleTextConfig;
}
