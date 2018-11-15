#include <string>
#include <vector>
#include <fstream>
#include <string.h>
#include <iostream>
#include <cstdlib>

using namespace std;

extern vector<uint8_t> compressImage(uint8_t* image, uint32_t sleepTime, uint16_t x_res, uint16_t y_res);

int main(int argc, char* argv[]) {
    //must be a raw, binary pbm file (Magic number P4)
    ifstream rawFile;
    rawFile.open(argv[1], ios::binary);

    //get image dimensions
    uint16_t x_res = atoi(argv[3]);
    uint16_t y_res = atoi(argv[4]);

    //get sleep time
    uint16_t sleepTime = atoi(argv[5]);

    //allocate memory for image buffer
    uint8_t* image = (uint8_t*) malloc(x_res*y_res/8);

    //read image
    rawFile.read((char*) image, x_res*y_res/8);
    rawFile.close();

    //invert image so it looks right on the screen
    for (int i = 0; i < x_res*y_res/8; i++) {
        image[i] = image[i]^0xff;
    }

    //generate compressed image
    vector<unsigned char> compressed = compressImage(image, sleepTime, x_res, y_res);

    free(image);

    ofstream(argv[2], ios::binary).write((const char*) compressed.data(), compressed.size());
    return 0;
}
