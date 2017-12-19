run:
	g++ *.cpp -o genimg -w -static
	cp genimg ../www/

debug:
	g++ *.cpp -g -o genimg
