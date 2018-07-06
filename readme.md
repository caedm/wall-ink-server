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
1. Install the following dependencies to a Linux server: ```git```, ```gcc```, ```go```, ```make``` (tested with GNU Make), MariaDB or MySQL
1. Clone this repo into a directory adjacent to your server's web root, which our makefile currently assumes is located in ```../www``` (this will hopefully be improved in the future)
1. Edit the credentials in ```wall-ink-server/web/config/settings.cfg.example``` and save the file as ```wall-ink-server/config/settings.cfg```
1. Create the table in mariadb or mysql with ```mysql dbNameHere < setup.sql```
1. Edit ```wall-ink-server/web/device_manager/.htaccess``` with your organization's information
1. (optional) If you want to use the built-in voltage monitoring tool, follow the steps below:
    1. Install the optional ```rrdtool``` dependency
    1. Edit the ```wall-ink-server/voltage_monitor/collectData.sh``` script to have correct filepaths
    1. Create a cron job, systemd timer, or similar to run the ```wall-ink-server/voltage_monitor/collectData.sh``` script once every 30 minutes
1. (optional) For those using Google Calendar, follow the steps below:
    1. Open a web browser and follow the steps on https://developers.google.com/calendar/quickstart/go to enable the Google Calendar API
    1. Save the ```client_secret.json``` file you got from the step above to ```wall-ink-server/image_gen/web/google/client_secret.json``` 
    1. Open terminal and navigate to ```wall-ink-server/image_gen/web/google```
    1. Run the command ```./gcal``` and follow the onscreen instructions
    1. Save the ```token.json``` file you got from the step above to ```wall-ink-server/image_gen/web/google/client_secret.json```
1. (optional) Edit the image key in ```compressImage.cpp```; don't forget to also edit the key in the Arduino sketch if you do this!
1. (optional) Follow the instructions under the **Integrating with other scheduling systems** header to create a plugin to integrate with your own calendaring system, if there isn't already one
1. (recommended) You'll probably want to change the website that the QR codes point to. This is found in image.cpp.
1. Follow the steps below to build the project

# Build
These instructions assume that your web root is hosted in a ```www``` folder adjancent to the folder the repo resides in.
To build and deploy to the test server (hosted at ```../www/test```), go to ```wall-ink-server/``` and use the ```make``` command. You will need ```gcc``` and GNU ```make```. To build and deploy to the live server (hosted at ```../www```), use the command ```make deploy``` instead.

After building, you'll want to point your Wall-Ink module at the server by changing the baseURL in the firmware.

# Integrating with other scheduling systems
If you want to integrate with a scheduling system other than Booked or Google Calendar, you need to create your own plugin. This isn't too hard! All your plugin needs to do is take a filename in as a parameter, then append some text to that file; for example:

```
CTB 450 Group Space 2
Yacht Club Meeting
2018-06-25 09:00:00
2018-06-25 13:30:00
Test reservation
2018-06-25 15:00:00
2018-06-25 15:30:00
```

The first line is the name of the room or resource being scheduled. This line is followed by information about any number of meetings or events. Each meeting is formatted in the following way:

* Line 1: The name of the meeting
* Line 2: The beginning time/date of the meeting, in the format shown above
* Line 3: The ending time/date of the meeting, in the format shown above

After you have created this, you need to edit the ```web/get_image.sh``` file to call your plugin for screens set to use it.

### Limitations:
If you integrate your own plugin, the device manager tool cannot be easily used to set the room or resource associated with the device. Of course, you could fix this by changing the following files:
 * ```web/device_manager/edit_device.php```
 * ```web/device_manager/view_devices.php```

# Device Manager
The Device Manager website is hosted at the web root. It acts as an aid in keeping track of your devices.
## Important Files
The files for the configuration website can be found at ```wall-ink-server/web/device_manager```.
#### dbconfig.php
Contains information for connecting to the database
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
Makes sure only those with proper credentials can access the site

# Image Generation and Hosting
The image generation code is located at ```wall-ink-server/```. GCC 8.1.0 was used to build it.
## Important Files
#### makefile
Builds the code and deploys it to the test server with ```make```; builds the code and deploys it to the live server with ```make deploy```. Builds the code with debug flags but does not deploy it with ```make debug```.
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
1. The current Unix time (4 bytes)
1. The Unix time when the device should next wake and contact the server (4 bytes)
1. A sha1 hash of the sha1 hash of the raw image buffer followed by the sha1 hash of the imagekey (20 bytes)
1. The value of the first pixel in the image
1. The run-length encoded image (explained below)

The images are compressed with a very simple run-length encoding algorithm. Each byte contains the number of pixels before a differing pixel is encountered. If the value is greater than 255, more than one byte will be used (for example, 255 255 16 for 526 identical pixels in a row). If the value is an exact multiple of 255, a zero will be appended (for example, 255 255 0 for 510 identical pixels in a row).  For example, this 16x16 image: 
![Picture of the number 5](https://i.imgur.com/71pE4rY.png)
would be encoded as this:
```
2c00 185b 3407 185b 9318 7c12 5b9f c496
ba16 663c d789 e0b6 a346 4269 0011 0e02
0406 0402 0401 0902 0401 0902 0404 0602
0401 0302 0402 0901 0402 0901 0402 0901
0402 0401 0401 0402 0401 0301 0502 0503
0602 0e02 0e11 0a                      
```

All values are little endian.

On a 7" screen, this reduces the image size from 30 kilobytes to about 5.5 kilobytes. Although there are other algorithms which can achieve better compression, this custom algorithm was used because it was relatively easy to implement and we weren't able to find any pre-made compression/decompression software which worked with the ESP8266.
