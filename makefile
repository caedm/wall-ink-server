run:
	g++ *.cpp -o genimg
	./make_images.sh

debug:
	g++ *.cpp -g -o genimg
