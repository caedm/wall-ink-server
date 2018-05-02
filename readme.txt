This directory contains stuff for the server, and the source code for the stuff running on the server.

The web directory contains the dev version of the website
the Adafruit-GFX-Library directory contains some libraries used mainly for font rendering
the Fonts directory contains some fonts
the qr_code_generator directory contains a library for generating QR codes
the voltage_monitor directory contains a script for updating the RRD databases which contain the voltage history of each physical device

Use "make" to build genimg & pbmToCompressed and to copy all relevant files to the ../www/test/ directory
Use "make deploy" to build genimg & pbmToCompressed and to copy all relevant files to the ../www/ directory

Up-to-date documentation can be found on the Admin:Door_Display page on the CAEDM wiki
