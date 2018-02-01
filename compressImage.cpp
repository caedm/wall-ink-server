#include <iostream>
#include "compressImage.h"

using namespace std;

uint8_t getPixel(unsigned long int x, unsigned long int y);

vector<uint8_t> compressImage(string* reservations, uint8_t* image, uint32_t sleepTime, uint16_t x_res, uint16_t y_res) {
    vector<uint8_t> compressed;
    compressed.clear();
    time_t currentTime = time(nullptr);
    uint8_t* compressedTime = (uint8_t*) malloc(4);
    uint8_t* nextTime = (uint8_t*) malloc(4);
    uint8_t* imageHash = (uint8_t*) malloc(4);
    *((uint32_t*) compressedTime) = currentTime;
    *((uint32_t*) nextTime) = *((uint32_t*) compressedTime) + sleepTime;
#if DEBUG == 1
    cout << hex;
    cout << "time size: " << sizeof(currentTime) << endl << "current time: " << currentTime << endl;
    cout << "compressed time: " << *((uint32_t*) compressedTime) << endl;
    cout << "next time: " << *((uint32_t*) nextTime) << endl;
    cout << "byte by byte: " << +compressedTime[0] << " " << +compressedTime[1] << " " << +compressedTime[2] << " " << +compressedTime[3] << endl;
#endif
    compressed.push_back(compressedTime[0]);
    compressed.push_back(compressedTime[1]);
    compressed.push_back(compressedTime[2]);
    compressed.push_back(compressedTime[3]);
    compressed.push_back(nextTime[0]);
    compressed.push_back(nextTime[1]);
    compressed.push_back(nextTime[2]);
    compressed.push_back(nextTime[3]);
    //generate image hash
    *((uint32_t*) imageHash) = 0;
    for (int i = 0; i < x_res * y_res/8; i++) {
        *((uint32_t*) imageHash) += (image[i] * (i+1))/2;
    }
    compressed.push_back(imageHash[0]);
    compressed.push_back(imageHash[1]);
    compressed.push_back(imageHash[2]);
    compressed.push_back(imageHash[3]);

    compressed.push_back(getPixel(0, 0));
    free(compressedTime);
    free(nextTime);
    uint32_t pointer = 0;
    uint8_t counter = 0;
    uint8_t lastEntry = getPixel(0,0);
    while (++pointer < x_res * y_res) {
       ++counter;
       if (counter == 0xff) {
           compressed.push_back(counter);
#if DEBUG == 1
           cout << (int) counter << " " << (int) lastEntry << " " << pointer % x_res << "," << pointer / x_res << endl;
#endif
           counter = 0;
       } else if (getPixel(pointer, 0) != lastEntry) {
           compressed.push_back(counter);
#if DEBUG == 1
           cout << (int) counter << " " << (int) lastEntry << " "<< pointer % x_res << "," << pointer / x_res << endl;
#endif
           counter = 0;
           lastEntry = getPixel(pointer, 0);
       }
    }
    compressed.push_back(++counter);
    return compressed;
}
