run:
	g++ image.cpp compressImage.cpp libqrencode/*.c -Wwrite-strings -fpermissive -pthread -o web/genimg -static
	#g++ -L/usr/local/lib/ image.cpp compressImage.cpp -o web/genimg -static
	#g++ image.cpp compressImage.cpp libqrencode/mqrspec.c libqrencode/bitstream.c libqrencode/qrinput.c libqrencode/rsecc.c libqrencode/mmask.c libqrencode/mask.c libqrencode/split.c libqrencode/qrspec.c libqrencode/qrencode.c -o web/genimg -static
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
	cp libqrencode.so.4 web/libqrencode.so.4
	cp web/get_image.php ../www/
	cp web/get_image.sh ../www/
	cp web/unix_time.php ../www/

test:
	g++ image.cpp compressImage.cpp libqrencode/*.c -Wwrite-strings -fpermissive -pthread -o web/genimg -static
	g++ pbmToCompressed.cpp compressImage.cpp -o pbmToCompressed -static

debug:
	g++ image.cpp compressImage.cpp -g -o web/genimg -static
	g++ pbmToCompressed.cpp compressImage.cpp -g -o pbmToCompressed -static
	cp web/genimg ../www/
