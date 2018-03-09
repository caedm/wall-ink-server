# Diagnostics. Adding '-fsanitize=address' is helpful for most versions of Clang and newer versions of GCC.
#CXXFLAGS += -Wall -fsanitize=undefined
# Link libraries statically
CXXFLAGS += -static -O1
LIBSRC = BitBuffer QrCode QrSegment compressImage
objects = image.o pbmToCompressed.o compressImage.o BitBuffer.o QrCode.o QrSegment.o
VPATH = qr_code_generator:web
CXX=g++


test: genimg pbmToCompressed

deploy: genimg pbmToCompressed
	rm -rf ../www/image_data
	mkdir ../www/image_data
	chmod g+w ../www/image_data
	rm -rf ../www/device_manager
	mkdir ../www/device_manager
	chmod g+w ../www/device_manager
	mkdir ../www/device_manager/css
	chmod g+w ../www/device_manager/css
	cp web/device_manager/css/view_devices.css ../www/device_manager/css/
	cp web/device_manager/css/edit_device.css ../www/device_manager/css/
	cp web/device_manager/dbconfig.php ../www/device_manager/
	cp web/device_manager/view_devices.php ../www/device_manager/
	cp web/device_manager/edit_device.php ../www/device_manager/
	cp web/device_manager/handle_edit_device.php ../www/device_manager/
	cp web/device_manager/handle_delete_device.php ../www/device_manager/
	cp web/device_manager/print_info.php ../www/device_manager/
	cp web/device_manager/.htaccess ../www/device_manager/
	cp web/index.html ../www/
	cp web/genimg ../www/
	cp web/get_image.php ../www/
	cp web/get_image.sh ../www/
	cp web/rawToPng.sh ../www/
	cp web/unix_time.php ../www/
	cp web/r.php ../www/

genimg : image.o compressImage.o BitBuffer.o QrCode.o QrSegment.o
	$(CXX) image.o $(LIBSRC:=.o) $(CXXFLAGS) -o web/genimg

pbmToCompressed : pbmToCompressed.o compressImage.o
	$(CXX) pbmToCompressed.o compressImage.o $(CXXFLAGS) -o pbmToCompressed

image.o : image.h qr_code_generator/QrCode.hpp
pbmToCompressed.o : pbmToCompressed.cpp compressImage.cpp compressImage.h
compressImage.o : compressImage.h
BitBuffer.o : BitBuffer.hpp
QrCode.o : QrCode.hpp
QrSegment.o : QrSegment.hpp

debug:
	$(CXX) image.cpp $(LIBSRC:=.cpp) $(CXXFLAGS) -g -o web/genimg
	$(CXX) pbmToCompressed.cpp compressImage.cpp $(CXXFLAGS) -g -o pbmToCompressed

clean : 
	rm $(objects)
