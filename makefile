run:
	g++ *.cpp -o web/genimg -w -static
	cp web/css/devices.css ../www/css/
	cp web/dbconfig.php ../www/
	cp web/dbconnection.php ../www/
	cp web/genimg ../www/
	cp web/get_image.php ../www/
	cp web/get_image.sh ../www/
	cp web/print_info.php ../www/
	cp web/unix_time.php ../www/

debug:
	g++ *.cpp -g -o genimg
