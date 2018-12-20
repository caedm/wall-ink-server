# Introduction
This project is a server-side companion to the [Wall-Ink](https://github.com/caedm/wall-ink) project. The wall-ink devices display any image on their e-ink screens that the wall-ink-server delivers to it.  The bulk of the server [plugins](https://github.com/caedm/wall-ink-server/wiki/Plugin-architecture) created to date are for displaying scheduling systems, but the wall-ink-server can be used for any digital signage purposes. Right now the server has plugins for Outlook, Google Calendar and the Booked scheduling system, but it could be integrated with any scheduling system that has a decent API.  There are other [plugins](https://github.com/caedm/wall-ink-server/wiki/Plugin-architecture) as well, and it is not too hard to create your own.

<img src="https://i.imgur.com/etozOAa.png" width="283"><img src="https://i.imgur.com/bR7Etyk.png" width="268"><img src="https://i.imgur.com/izWwPwq.png" width="268">

# Summary
The wall-ink-server houses several important functions:

* It generates new images for the [Wall-Ink](https://github.com/caedm/wall-ink) displays
* It serves images to the Wall-Ink displays
* It notifies the database when the Wall-Ink displays are updated
* It hosts a website for the configuration of the Wall-Ink displays

# Installation
1. Install the following dependencies to a Linux server: ```git```, ```gcc```, ```make``` (tested with GNU Make), MariaDB or MySQL
1. Clone this repo ```git clone https://github.com/caedm/wall-ink-server```
1. Edit the information in ```wall-ink-server/web/config/settings.cfg.example``` and save the file as ```wall-ink-server/web/config/settings.cfg```  See the wiki article on [settings.cfg](https://github.com/caedm/wall-ink-server/wiki/settings.cfg) for a full explanation of each setting.
1. Create the table in mariadb or mysql by running the ```setup_sql.sh``` script in the wall-ink-server directory.  This script will only be successful if valid device database settings were set in the settings.cfg file above. 
1. Edit ```wall-ink-server/web/device_manager/.htaccess``` with your organization's information.  Settings in .htaccess files protect your database passwords and other configuration settings from being world readable on the wall-ink-server web site, and this particular one also restricts access to the device manager website.
1. Edit the image key in the Arduino sketch to match the image key in the ```wall-ink-server/web/config/settings.cfg``` file. See the wiki article on [image security](https://github.com/caedm/wall-ink-server/wiki/image_security) to understand the use and purpose of the image key. Point your Wall-Ink module at the server by changing the baseURL in the firmware.
1. (optional) If you want to use the built-in [voltage monitoring tool](https://github.com/caedm/wall-ink-server/wiki/voltage-monitoring), follow the steps below:
    1. Install the optional ```rrdtool``` dependency
    1. Edit the ```wall-ink-server/voltage_monitor/collectData.sh``` script to source the correct web/config/database.sh file with your settings in them.  This should be on line 3 of collectData.sh.
    1. Create a cron job, systemd timer, or similar to run the ```wall-ink-server/voltage_monitor/collectData.sh``` script once every 30 minutes
1. (optional) For those using Google Calendar, follow the steps below:
    1. Install the dependency ```php``` necessary run the google API key software from the command line
    1. Open a web browser and follow the steps on https://developers.google.com/calendar/quickstart/php to enable the Google Calendar API
    1. Save the ```credentials.json``` file you got from the step above to ```wall-ink-server/web/plugin_dependencies/google/credentials.json``` 
    1. Open terminal and navigate to ```wall-ink-server/web/plugin_dependencies/google```
    1. Run the command ```php ./quickstart.php``` from a terminal and follow the onscreen instructions.  It will have you paste a url in to a browser to activate the API key.  You will copy a line of text back from google and past it back into the quickstart.php script on the command line.  
    1. The quickstart.php file should now create a new .json file ```wall-ink-server/web/plugin_dependencies/google/token.json``` using both the credentials.json file you downloaded, and the phrase pasted into the quickstart.php script.  If the API keys were successfully created, running quickstart.php should display a list of calendars available from the google API to the command line.  You can use the quickstart.php script from the command line at any time to verify that the google API keys are still working.
1. (optional) Create a [plugin](https://github.com/caedm/wall-ink-server/wiki/Plugin-architecture) to integrate with your own calendaring system, or for a whole different use case like a weather station or a bus schedule.
1. Follow the steps below to build the project

# Build
You will need ```gcc``` and GNU ```make```. To build and deploy to the live server (hosted at your web directory as defined in settings.cfg), use the command ```make```.

# Wiki
This project has a fairly extensive [Wiki](https://github.com/caedm/wall-ink-server/wiki), including articles on [Troubleshooting](https://github.com/caedm/wall-ink-server/wiki/Troubleshooting), the [database](https://github.com/caedm/wall-ink-server/wiki/Database), [Plugins](https://github.com/caedm/wall-ink-server/wiki/Plugin-architecture), [Nagios monitoring](https://github.com/caedm/wall-ink-server/wiki/Nagios-monitoring), and many more.  The companion [wall-ink device project](https://github.com/caedm/wall-ink) also has a great deal of documentatiion in it's [Wiki](https://github.com/caedm/wall-ink/wiki) as well.

# Plugins
The wall-ink-server can deliver any image to a wall-ink device.  [Plugins](https://github.com/caedm/wall-ink-server/wiki/Plugin-architecture) allow [additional code](https://github.com/caedm/wall-ink-server/wiki/Coding-a-new-plugin) to be added to the wall-ink-server to add integration of any API or code to generate images specific to your application.  

A [special trivial case for adding a new plugin would be to add an additional "scheduler"](https://github.com/caedm/wall-ink-server/wiki/Plugin-architecture#what-is-a-scheduler-plugin) like Outlook, or iCal, etc.  The difficult part of writing an engine for creating images for a schedule on a wall-ink device has already been written, and is in use by several plugins that are already part of the wall-ink-server project.  All that is necesary is to make API calls to facilitate a few functions that format text. 

# Device Manager
The [Device Manager](https://github.com/caedm/wall-ink-server/wiki/device-manager) website is hosted at the web root. It is used as a configuration center for your wall-ink devices.  A list of the [important files in the device manager](https://github.com/caedm/wall-ink-server/wiki/Device-manager#important-files) and their purpose is located in the Wiki.


<img src="https://i.imgur.com/QGNbDXT.png" height="300">


# Image Generation and Hosting
The image generation code is located at ```wall-ink-server/```. GCC 8.1.0 was used to build it. See the discussion on [Wall ink image formats and image handling](https://github.com/caedm/wall-ink-server/wiki/Wall-ink-image-formats-and-image-handling) for more information.  A list of [important files in image generation](https://github.com/caedm/wall-ink-server/wiki/Wall-ink-image-formats-and-image-handling#important-files) and their purpose is also available in the Wiki.

<img src= "https://i.imgur.com/NNI1ogo.png"  width="283">



