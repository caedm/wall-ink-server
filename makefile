# Diagnostics. Adding '-fsanitize=address' is helpful for most versions of Clang and newer versions of GCC.
#CXXFLAGS += -Wall -fsanitize=undefined
# Link libraries statically
CXXFLAGS += -static
LIBSRC = qr_code_generator/BitBuffer.cpp qr_code_generator/QrCode.cpp qr_code_generator/QrSegment.cpp compressImage.cpp


run:
	g++ image.cpp $(LIBSRC) $(CXXFLAGS) -o web/genimg -O1
	g++ pbmToCompressed.cpp compressImage.cpp -o pbmToCompressed -static
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
	cp web/unix_time.php ../www/
	cp web/r.php ../www/

test:
	g++ image.cpp $(LIBSRC) $(CXXFLAGS) -o web/genimg -O1
	g++ pbmToCompressed.cpp compressImage.cpp -o pbmToCompressed -static -O1

debug:
	g++ image.cpp $(LIBSRC) $(CXXFLAGS) -o web/genimg -Og -g
	g++ pbmToCompressed.cpp compressImage.cpp -g -o pbmToCompressed -static -Og
