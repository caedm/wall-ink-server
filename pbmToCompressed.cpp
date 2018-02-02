#include <string>
#include <vector>
#include <fstream>
#include <string.h>
#include <iostream>

using namespace std;

vector<uint8_t> compressImage(uint8_t* image, uint32_t sleepTime, uint16_t x_res, uint16_t y_res);

int main(int argc, char* argv[]) {
    //must be a raw, binary pbm file (Magic number P4)
    ifstream pbmFile;
    pbmFile.open(argv[1], ios::binary);

    //get some junk out of the way
    string dimensions;
    getline(pbmFile, dimensions);
    getline(pbmFile, dimensions);

    //get image dimensions
    getline(pbmFile, dimensions);
    uint16_t x_res = stoi(dimensions.substr(0, dimensions.find(' ')));
    uint16_t y_res = stoi(dimensions.substr(dimensions.find(' ') + 1, dimensions.npos));

    //allocate memory for image buffer
    uint8_t* image = (uint8_t*) malloc(x_res*y_res/8);

    //read image
    pbmFile.read((char*) image, x_res*y_res/8);
    pbmFile.close();

    //invert image so it looks right on the screen
    for (int i = 0; i < x_res*y_res/8; i++) {
        image[i] = image[i]^0xff;
    }

    //generate compressed image
    vector<unsigned char> compressed = compressImage(image, 1800, x_res, y_res);

    free(image);

    ofstream(argv[2], ios::binary).write((const char*) compressed.data(), compressed.size());
    return 0;
}
