#!/bin/bash

echo "Checkking settings file..."

# Check if wall-ink-server settings are set
if [ -z ${buildTimeWebDirectory+x} ]; then echo "** Config Error ** buildTimeWebDirectory not set in settings.cfg"; fi
if [ -z ${runTimeWebDirectory+x} ]; then echo "** Config Error ** runTimeWebDirectory not set in settings.cfg"; fi
if [ -z ${rrdDirectory+x} ]; then echo "** Config Error ** rrdDirectory not set in settings.cfg"; fi
if [ -z ${debugModeActive+x} ]; then echo "** Config Error ** debugModeActive not set in settings.cfg"; fi
if [ -z ${imageKey+x} ]; then echo "** Config Error ** imageKey not set in settings.cfg"; fi
if [ -z ${timeZone+x} ]; then echo "** Config Error ** timeZone not set in settings.cfg"; fi

# Check if wall-ink device database settings are set
if [ -z ${deviceDatabaseServer+x} ]; then echo "** Config Error ** deviceDatabaseServer not set in settings.cfg"; fi
if [ -z ${deviceDatabaseUsername+x} ]; then echo "** Config Error ** deviceDatabaseUsername not set in settings.cfg"; fi
if [ -z ${deviceDatabasePassword+x} ]; then echo "** Config Error ** deviceDatabasePassword not set in settings.cfg"; fi
if [ -z ${deviceDatabaseName+x} ]; then echo "** Config Error ** deviceDatabaseName not set in settings.cfg"; fi

# Check if Example scheduler plugin settings are set
if [ -z ${exampleSchedulerActive+x} ]; then 
	echo "** Config Error ** exampleSchedulerActive not set in settings.cfg" 
elif [ $exampleSchedulerActive == "true" ]; then
	echo "Example Scheduler Plugin Active"
	if [ -z ${exampleSchedulerDisplayUrl+x} ]; then echo "** Config Error ** exampleSchedulerDisplayUrl not set in settings.cfg"; fi
	if [ -z ${exampleSchedulerQrCodeBaseUrlBeginning+x} ]; then echo "** Config Error ** exampleSchedulerQrCodeBaseUrlBeginning not set in settings.cfg"; fi
	if [ -z ${exampleSchedulerQrCodeBaseUrlEnd+x} ]; then echo "** Config Error ** exampleSchedulerQrCodeBaseUrlEnd not set in settings.cfg"; fi
else	echo "Example Scheduler Plugin Not Active"
fi

# Check if Google Calendar plugin settings are set
if [ -z ${googleIntegrationActive+x} ]; then 
	echo "** Config Error ** googleIntegrationActive not set in settings.cfg" 
elif [ $googleIntegrationActive == "true" ]; then
	echo "Google Calendar Plugin Active"
	if [ -z ${googleCalendarDisplayUrl+x} ]; then echo "** Config Error ** googleCalendarDisplayUrl not set in settings.cfg"; fi
	if [ -z ${googleCalendarQrCodeBaseUrlBeginning+x} ]; then echo "** Config Error ** googleCalendarQrCodeBaseUrlBeginning not set in settings.cfg"; fi
	if [ -z ${googleCalendarQrCodeBaseUrlEnd+x} ]; then echo "** Config Error ** googleCalendarQrCodeBaseUrlEnd not set in settings.cfg"; fi
else	echo "Google Calendar Plugin Not Active"
fi

# Check if Booked plugin settings are set
if [ -z ${bookedIntegrationActive+x} ]; then 
	echo "** Config Error ** bookedIntegrationActive not set in settings.cfg" 
elif [ $bookedIntegrationActive == "true" ]; then
	echo "Google Calendar Plugin Active"
	if [ -z ${bookedDatabaseServer+x} ]; then echo "** Config Error ** bookedDatabaseServer not set in settings.cfg"; fi
	if [ -z ${bookedDatabaseUsername+x} ]; then echo "** Config Error ** bookedDatabaseUsername not set in settings.cfg"; fi
	if [ -z ${bookedDatabasePassword+x} ]; then echo "** Config Error ** bookedDatabasePassword not set in settings.cfg"; fi
	if [ -z ${bookedDatabaseName+x} ]; then echo "** Config Error ** bookedDatabaseName not set in settings.cfg"; fi
	if [ -z ${bookedDisplayUrl+x} ]; then echo "** Config Error ** bookedDisplayUrl not set in settings.cfg"; fi
	if [ -z ${bookedQrCodeBaseUrlBeginning+x} ]; then echo "** Config Error ** bookedQrCodeBaseUrlBeginning not set in settings.cfg"; fi
	if [ -z ${bookedQrCodeBaseUrlEnd+x} ]; then echo "** Config Error ** bookedQrCodeBaseUrlEnd not set in settings.cfg"; fi
else	echo "Google Calendar Plugin Not Active"
fi

# Check if Static Images plugin settings are set
if [ -z ${staticImagesPluginActive+x} ]; then 
	echo "** Config Error ** staticImagesPluginActive not set in settings.cfg" 
elif [ $staticImagesPluginActive == "true" ]; then
	echo "Static Images Plugin Active"
else	echo "Example Scheduler Plugin Not Active"
fi

# Check if SimpleText plugin settings are set
if [ -z ${simpleTextPluginActive+x} ]; then 
	echo "** Config Error ** simpleTextPluginActive not set in settings.cfg" 
elif [ $simpleTextPluginActive == "true" ]; then
	echo "Simple Text Plugin Active"
else	echo "Simple Text Plugin Not Active.  Some error messages will unable to be displayed on wall-ink devices."
fi

# Check if icalWeb plugin settings are set
if [ -z ${icalWebPluginActive+x} ]; then 
	echo "** Config Error ** icalWebPluginActive not set in settings.cfg" 
elif [ $icalWebPluginActive == "true" ]; then
	echo "icalWeb Plugin Active"
	if [ -z ${icalWebQrCodeBaseUrlBeginning+x} ]; then echo "** Config Error ** icalWebQrCodeBaseUrlBeginning not set in settings.cfg"; fi
	if [ -z ${icalWebQrCodeBaseUrlEnd+x} ]; then echo "** Config Error ** icalWebQrCodeBaseUrlEnd not set in settings.cfg"; fi
else	echo "icalWeb Plugin Not Active"
fi
