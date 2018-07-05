# Diagnostics. Adding '-fsanitize=address' is helpful for most versions of Clang and newer versions of GCC.
#CXXFLAGS += -Wall -fsanitize=undefined
# Link libraries statically
CXXFLAGS += -static -O1 -g -std=c++11
LIBSRC = BitBuffer QrCode QrSegment compressImage sha1 layouts Adafruit_GFX
objects = image.o pbmToCompressed.o compressImage.o BitBuffer.o QrCode.o QrSegment.o sha1.o layouts.o Adafruit_GFX.o
VPATH = qr_code_generator:web:Adafruit-GFX-Library
CXX=g++
CC=gcc
CFLAGS += -static


test: genimg pbmToCompressed
	rm -rf ../www/test/
	mkdir ../www/test
	chmod g+w ../www/test
	mkdir ../www/test/image_data
	chmod g+w ../www/test/image_data
	mkdir ../www/test/voltage_monitor
	chmod g+w ../www/test/voltage_monitor
	mkdir ../www/test/voltage_monitor/data
	chmod g+w ../www/test/voltage_monitor/data
	mkdir ../www/test/device_manager
	chmod g+w ../www/test/device_manager
	mkdir ../www/test/device_manager/css
	chmod g+w ../www/test/device_manager/css
	mkdir ../www/test/device_manager/js
	chmod g+w ../www/test/device_manager/js
	cp -r web/google ../www/test/google
	chmod g+w ../www/test/google/token.json
	cp web/.htaccess ../www/test/
	cp web/device_manager/css/view_devices.css ../www/test/device_manager/css/
	cp web/device_manager/css/edit_device.css ../www/test/device_manager/css/
	cp web/device_manager/css/voltage_charts.css ../www/test/device_manager/css/
	cp web/device_manager/js/edit_device.js ../www/test/device_manager/js/
	cp web/device_manager/js/view_devices.js ../www/test/device_manager/js/
	cp web/device_manager/dbconfig.php ../www/test/device_manager/
	cp web/device_manager/view_devices.php ../www/test/device_manager/
	cp web/device_manager/edit_device.php ../www/test/device_manager/
	cp web/device_manager/handle_edit_device.php ../www/test/device_manager/
	cp web/device_manager/handle_delete_device.php ../www/test/device_manager/
	cp web/device_manager/print_info.php ../www/test/device_manager/
	cp web/device_manager/handle_upload.php ../www/test/device_manager/
	cp web/device_manager/voltage_charts.php ../www/test/device_manager/
	cp web/device_manager/.htaccess ../www/test/device_manager/
	cp web/index.html ../www/test/
	cp web/genimg ../www/test/
	cp web/pbmToCompressed ../www/test/
	cp web/get_image.php ../www/test/
	cp web/get_image.sh ../www/test/
	cp web/booked.sh ../www/test/
	cp web/get_png.php ../www/test/
	cp web/rawToPng.sh ../www/test/
	cp web/unix_time.php ../www/test/
	cp web/r.php ../www/test/

deploy: genimg pbmToCompressed
	rm -rf ../www/image_data
	mkdir ../www/image_data
	chmod g+w ../www/image_data
	rm -rf ../www/voltage_monitor
	mkdir ../www/voltage_monitor
	chmod g+w ../www/voltage_monitor
	rm -rf ../www/voltage_monitor/data
	mkdir ../www/voltage_monitor/data
	chmod g+w ../www/voltage_monitor/data
	rm -rf ../www/device_manager
	mkdir ../www/device_manager
	chmod g+w ../www/device_manager
	mkdir ../www/device_manager/css
	chmod g+w ../www/device_manager/css
	mkdir ../www/device_manager/js
	chmod g+w ../www/device_manager/js
	cp -r web/google ../www/google
	chmod g+w ../www/google/token.json
	cp web/.htaccess ../www/
	cp web/device_manager/css/view_devices.css ../www/device_manager/css/
	cp web/device_manager/css/edit_device.css ../www/device_manager/css/
	cp web/device_manager/css/voltage_charts.css ../www/device_manager/css/
	cp web/device_manager/js/edit_device.js ../www/device_manager/js/
	cp web/device_manager/js/view_devices.js ../www/device_manager/js/
	cp web/device_manager/dbconfig.php ../www/device_manager/
	cp web/device_manager/view_devices.php ../www/device_manager/
	cp web/device_manager/edit_device.php ../www/device_manager/
	cp web/device_manager/handle_edit_device.php ../www/device_manager/
	cp web/device_manager/handle_delete_device.php ../www/device_manager/
	cp web/device_manager/print_info.php ../www/device_manager/
	cp web/device_manager/handle_upload.php ../www/device_manager/
	cp web/device_manager/voltage_charts.php ../www/device_manager/
	cp web/device_manager/.htaccess ../www/device_manager/
	cp web/index.html ../www/
	cp web/genimg ../www/
	cp web/pbmToCompressed ../www/
	cp web/get_image.php ../www/
	cp web/get_image.sh ../www/
	cp web/booked.sh ../www/
	cp web/get_png.php ../www/
	cp web/rawToPng.sh ../www/
	cp web/unix_time.php ../www/
	cp web/r.php ../www/

genimg : image.o compressImage.o BitBuffer.o QrCode.o QrSegment.o layouts.o Adafruit_GFX.o
	$(CXX) image.o $(LIBSRC:=.o) $(CXXFLAGS) -o web/genimg

pbmToCompressed : pbmToCompressed.o compressImage.o
	$(CXX) pbmToCompressed.o compressImage.o sha1.o $(CXXFLAGS) -o web/pbmToCompressed

image.o : image.h qr_code_generator/QrCode.hpp layouts.o Adafruit_GFX.o
pbmToCompressed.o : pbmToCompressed.cpp compressImage.cpp compressImage.h
compressImage.o : compressImage.h sha1.o
BitBuffer.o : BitBuffer.hpp
QrCode.o : QrCode.hpp
QrSegment.o : QrSegment.hpp
sha1.o : sha1.h
layouts.o : layouts.h
Adafruit_GFX.o : Adafruit_GFX.h

debug:
	$(CXX) image.cpp $(LIBSRC:=.cpp) $(CXXFLAGS) -g -o web/genimg
	$(CXX) pbmToCompressed.cpp compressImage.cpp $(CXXFLAGS) -g -o web/pbmToCompressed

clean : 
	rm $(objects)
