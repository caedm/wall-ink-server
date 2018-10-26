# Introduction
This project is a server-side companion to the [Wall-Ink](https://github.com/caedm/wall-ink) project. The focus is on scheduling systems, but it could be used for other digital signage purposes also. Right now it integrates with the Booked scheduling system and with Google Calendar, but it could be integrated with any scheduling system that has a decent API.

# Summary
The door display server houses several important functions:

* It generates new images for the [Wall-Ink](https://github.com/caedm/wall-ink) displays
* It serves images to the Wall-Ink displays
* It notifies the database when the Wall-Ink displays are updated
* It hosts a website for the configuration of the Wall-Ink displays

The following diagram roughly illustrates the information passed between the parts of the system:

![Door display sequence diagram](https://i.imgur.com/YZ32F0h.png)

# Installation
1. Install the following dependencies to a Linux server: ```git```, ```gcc```, ```make``` (tested with GNU Make), MariaDB or MySQL
1. Clone this repo ```git clone https://github.com/caedm/wall-ink-server```
1. Edit the information in ```wall-ink-server/web/config/settings.cfg.example``` and save the file as ```wall-ink-server/web/config/settings.cfg```  See the wiki article on [settings.cfg](https://github.com/caedm/wall-ink-server/wiki/settings.cfg) for a full explanation of each setting.
1. Create the table in mariadb or mysql by running the setup_sql.sh script in the wall-ink-server directory ```./setup_sql.sh```.  This script will only be successful if valid device database settings were set in the settings.cfg file above. 
1. Edit ```wall-ink-server/web/device_manager/.htaccess``` with your organization's information.  Settings in .htaccess files protect your database passwords and other configuration settings from being world readable on the wall-ink-server web site.
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
1. Copy the ```key.h.example``` to ```key.h``` and edit the file with your image key; don't forget to also edit the key in the Arduino sketch!  See the wiki article on [image security](https://github.com/caedm/wall-ink-server/wiki/image_security) to understand the use and purpose of key.h
1. (optional) Follow the instructions under the **Integrating with other scheduling systems** header to create a plugin to integrate with your own calendaring system.
1. Follow the steps below to build the project

# Build
To build and deploy to the test server (hosted at ```$webdirectory/test```), go to ```wall-ink-server/``` and use the ```make``` command. You will need ```gcc``` and GNU ```make```. To build and deploy to the live server (hosted at your web directory as defined in settings.cfg), use the command ```make deploy``` instead.

After building, you'll want to point your Wall-Ink module at the server by changing the baseURL in the firmware.

# Integrating with other scheduling systems
If you want to integrate with a scheduling system other than Booked or Google Calendar, you need to create your own plugin. This isn't too hard! All your plugin needs to do is implement the following interface in PHP:
```
interface iPlugin {
    public function getIndex();
    public function getName();
    public function isActive($config);
    public function getResources($config);
    public function getSchedule($config, $resourceId);
    public function getImage($config, $device);
    public function getDeviceType($device);
}
```
You then need to add an instance of your plugin to the ```plugins``` array defined in the web/plugin_dependencies/iPlugin.php file. When you've implemented all this, place your plugin in the web/plugins/ directory. Any settings relevant to your plugin (such as whether it is active) can be added to the web/config/settings.cfg file. Here are some details about the different functions you'll need to implement:

#### getIndex()
This function needs to return a non-negative integer that is not the same as the integer returned by any of the other plugins. A single or double-digit number is fine, just check first that you won't be colliding with other plugins.  This non-negative integer will be used by the device manager database to keep track of which wall-ink device is associated with which plugin.

#### getName()
This function returns a string containing the name of your plugin. For example, you might use ```return 'Exchange';```  This string is used in the device manager in the drop-down list of plugins to choose from.

#### isActive($config)
This function returns either the string "true" or the string "false". This is an aid for debugging purposes to turn plugins on and off.  There may be unexpected results if devices exist in the device manager database for a particular plugin, and that plugin is either inactive or removed.

#### getResources($config)
This function returns an array with resource IDs (a unique ID corresponding to a scheduleable resource) as the keys and resource names as the values.  This information is used to fill the drop down menu in the device manager when selecting which resource will be associated with a particular wall-ink device.

#### getSchedule($config, $resourceId)
This function will gather data from your scheduling software and format it in a specific manner that will be used to generate images that a wall-ink device can display.  The schedule returned from getSchedule will be passed into the scheduleGetImage() function for further processing.

The assumption of the engine that creates the images as seen below, is that the schedule returned by getSchedule() is just for the current date.  

This getSchedule() function must return a string formatted like the one below:
```
CTB 450 Group Space 2
Yacht Club Meeting
2018-06-25 09:00:00
2018-06-25 13:30:00
Test reservation
2018-06-25 15:00:00
2018-06-25 15:30:00
```
The first line is the name of the room or resource being scheduled. This line is followed by information about any number of events, meetings, or other reservations. Each reservation is formatted in the following way:

* Line 1: The name of the reservation
* Line 2: The beginning time/date of the reservation, in the format shown above
* Line 3: The ending time/date of the reservation, in the format shown above



#### getImage($config, $device)
This function takes in some information about a wall-ink device, and returns the file directory of the image that will be sent to the screen. The image needs to be made in a very specific format. If you're implementing a scheduling plugin, you'll probably be able to copy/paste an existing plugin's getImage function. If you run into difficulties, please contact one of the project's developers.

#### getDeviceType($device)
This function takes in some information about a wall-ink device, and returns some HTML that will be displayed on the edit_device page to help an admin choose what type of device it is (the name of this function is a little confusing). If you're implementing a scheduling plugin, you'll probably be able to copy/paste an existing plugin's getDeviceType function. If you run into difficulties, please contact one of the project's developers.

# Device Manager
The [Device Manager](https://github.com/caedm/wall-ink-server/wiki/device-manager) website is hosted at the web root. It is used a a configuration center for your wall-ink devices.

## Important Files
The files for the configuration website can be found at ```wall-ink-server/web/device_manager```
#### view_devices.php
This is the homepage; it shows a list of devices
#### edit_device
This file displays an HTML form to the user to allow them to edit the database entry for the device
#### handle_edit_device
A handler for the HTML form generated by ```edit_device.php```
#### handle_delete_device
A handler for the "Delete" button on ```edit_device.php```
#### handle_upload
A handler for the "Upload" button on ```edit_device.php```
#### .htaccess
Makes sure only those with proper credentials can access the site; needs to be edited to include accurate information about your organization

# Image Generation and Hosting
The image generation code is located at ```wall-ink-server/```. GCC 8.1.0 was used to build it.
## Important Files
#### makefile
Builds the code and deploys it to the test server with ```make```; builds the code and deploys it to the live server with ```make deploy```
#### web/config/settings.cfg
Contains configuration for the database connections; must run ```make``` after each edit
#### layouts.cpp
Contains the code used to generate individual image layouts
#### image.cpp
Contains many of the libraries used by ```layouts.cpp``` to generate the images from reservation data.
#### compressImage.cpp
Contains the code used to convert an array of bytes into a compressed image for the use of a display
#### fonts.h
Contains include statements for lots of Adafruit fonts so they don't need to be in image.cpp or image.h
#### letters.h
Contains a font that was found on stackoverflow; might be good to get rid of this.
#### sha1.c, sha1.h
Contains a library for sha1 hashing.
#### pbmToCompressed.cpp
Source code to convert a raw, binary .pbm file to a compressed file for the use of a display. Will not work with ASCII .pbm files!
#### qr_code_generator/
Contains a C++ library used by image.cpp to generate QR codes from strings
#### Adafruit-GFX-Library/
Contains a library used main for fonts in image.cpp
#### get_image.php
Takes in a MAC address and a voltage. If the MAC address corresponds to a device which needs a static image (device_type 5), it updates the ```last_checked_in``` and (if relevant) ```batteries_replaced_date``` fields on the database. It then and serves up the static image at ```../www/image_data/$mac_address.static```. Otherwise, it passes the MAC address and voltage to ```get_image.sh```.
#### get_image.sh
Takes in a MAC address and voltage, queries the database for information about appointments, and calls ```genimg```. It also updates the ```last_checked_in``` and (if relevant) ```batteries_replaced_date``` fields on the database.
#### genimg
Binary Linux executable which takes in a file containing information about a screen & its associated room and spits out a compressed image for use on the displays. It uses statically linked libraries, so it should run on most Linux systems.
#### pbmToCompressed
Binary Linux executable which takes in a raw, binary .pbm image and outputs a compressed file for use on the displays. Usage is:
  ```./pbmToCompressed image.pbm outputImage.static```
Must supply an image with the precisely correct resolution for the target display!

# Image file format
The contents of the file sent to the screen are as follows:
1. A sha1 hash of the sha1 hash of the next 8 bytes (2 & 3) followed by the sha1 hash of the imagekey (20 bytes)
1. The current Unix time (4 bytes)
1. The Unix time when the device should next wake and contact the server (4 bytes)
1. A sha1 hash of the sha1 hash of the raw image buffer followed by the sha1 hash of the imagekey (20 bytes)
1. The value of the first pixel in the image (1 byte)
1. The run-length encoded image (explained below)

The images are compressed with a very simple run-length encoding algorithm. Each byte contains the number of pixels before a differing pixel is encountered. If the value is greater than 255, more than one byte will be used (for example, 255 255 16 for 526 identical pixels in a row). If the value is an exact multiple of 255, a zero will be appended (for example, 255 255 0 for 510 identical pixels in a row).  For example, this 16x16 image: 
![Picture of the number 5](https://i.imgur.com/71pE4rY.png)
would be encoded as this:
```
9008 9e7f 544f 8bb6 d3c8 ba22 9790 b894
6ee7 38b6 2c00 185b 3407 185b 9318 7c12
5b9f c496 ba16 663c d789 e0b6 a346 4269
0011 0e02 0406 0402 0401 0902 0401 0902
0404 0602 0401 0302 0402 0901 0402 0901
0402 0901 0402 0401 0401 0402 0401 0301
0502 0503 0602 0e02 0e11 0a                      
```

All values are little endian.

On a 7" screen, this reduces the image size from 30 kilobytes to about 5.5 kilobytes. Although there are other algorithms which can achieve better compression, this custom algorithm was used because it was relatively easy to implement and we weren't able to find any pre-made compression/decompression software which worked with the ESP8266.
