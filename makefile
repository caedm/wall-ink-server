run:
	g++ *.cpp -o web/genimg -w -static
	rm -rf ../www/image_data
	mkdir ../www/image_data
	chmod g+w ../www/image_data
	rm -rf ../www/css
	mkdir ../www/css
	chmod g+w ../www/css
	cp web/css/devices.css ../www/css/
	cp web/dbconfig.php ../www/
	cp web/view_devices.php ../www/
	cp web/edit_device.php ../www/
	cp web/genimg ../www/
	cp web/get_image.php ../www/
	cp web/get_image.sh ../www/
	cp web/print_info.php ../www/
	cp web/unix_time.php ../www/

debug:
	g++ *.cpp -g -o genimg
