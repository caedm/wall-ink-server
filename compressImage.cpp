#include <iostream>
#include "compressImage.h"
#include "sha1.h"
#include <bitset>
#include <cstring>

#define IMAGE_KEY "hunter2"

using namespace std;

uint8_t getPixel(unsigned long int x, unsigned long int y, uint16_t x_res, uint16_t y_res, uint8_t* image) {
    return (image[x/8 + x_res*y/8] >> (7 - x%8)) & 0x01;
}

vector<uint8_t> compressImage(uint8_t* image, uint32_t sleepTime, uint16_t x_res, uint16_t y_res) {
    vector<uint8_t> compressed;
    compressed.clear();
    time_t currentTime = time(nullptr);
    uint8_t* compressedTime = (uint8_t*) malloc(4);
    uint8_t* nextTime = (uint8_t*) malloc(4);
    uint8_t* time = (uint8_t*) malloc(8);
    *((uint32_t*) compressedTime) = currentTime;
    *((uint32_t*) nextTime) = *((uint32_t*) compressedTime) + sleepTime;
    memcpy(time, compressedTime, 4);
    memcpy(time+4, nextTime, 4);
#if DEBUG == 1
    cout << hex;
    cout << "time size: " << sizeof(currentTime) << endl << "current time: " << currentTime << endl;
    cout << "compressed time: " << *((uint32_t*) compressedTime) << endl;
    cout << "next time: " << *((uint32_t*) nextTime) << endl;
    cout << "byte by byte: " << +compressedTime[0] << " " << +compressedTime[1] << " " << +compressedTime[2] << " " << +compressedTime[3] << endl;
#endif
    uint8_t timeHash[40];
    SHA1Context sha;
    int err;
    err = SHA1Reset(&sha);
    err = SHA1Input(&sha, time, 8);
    err = SHA1Result(&sha, timeHash);
    char imageKey[] = IMAGE_KEY;
    err = SHA1Reset(&sha);
    err = SHA1Input(&sha, (uint8_t*) imageKey, sizeof(imageKey));
    err = SHA1Result(&sha, timeHash+20);
    uint8_t finalTimeHash[20];
    err = SHA1Reset(&sha);
    err = SHA1Input(&sha, (uint8_t*) timeHash, 40);
    err = SHA1Result(&sha, finalTimeHash);
    for (int i = 0; i < 20; i++)
        compressed.push_back(finalTimeHash[i]);
    compressed.push_back(compressedTime[0]);
    compressed.push_back(compressedTime[1]);
    compressed.push_back(compressedTime[2]);
    compressed.push_back(compressedTime[3]);
    compressed.push_back(nextTime[0]);
    compressed.push_back(nextTime[1]);
    compressed.push_back(nextTime[2]);
    compressed.push_back(nextTime[3]);
    //generate image hash
    uint8_t imageHash[40];
    err = SHA1Reset(&sha);
    err = SHA1Input(&sha, image, x_res*y_res/8);
    err = SHA1Result(&sha, imageHash);
    err = SHA1Reset(&sha);
    err = SHA1Input(&sha, (uint8_t*) imageKey, sizeof(imageKey));
    err = SHA1Result(&sha, imageHash+20);
    uint8_t finalHash[20];
    err = SHA1Reset(&sha);
    err = SHA1Input(&sha, (uint8_t*) imageHash, 40);
    err = SHA1Result(&sha, finalHash);
    if (err)
    {
        fprintf(stderr,
        "SHA1Result Error %d, could not compute message digest.\n",
        err );
    }
    else
    {
        printf("\t");
        for(int i = 0; i < 20 ; ++i)
        {
            printf("%02X ", imageHash[i]);
        }
        printf("\n");
    }
    for (int i = 0; i < 20; i++)
        compressed.push_back(finalHash[i]);

    compressed.push_back(getPixel(0, 0, x_res, y_res, image));
    free(compressedTime);
    free(nextTime);
    uint32_t pointer = 0;
    uint8_t counter = 0;
    uint8_t lastEntry = getPixel(0,0, x_res, y_res, image);
    while (++pointer < x_res * y_res) {
       ++counter;
       if (counter == 0xff) {
           compressed.push_back(counter);
#if DEBUG == 1
           cout << (int) counter << " " << (int) lastEntry << " " << pointer % x_res << "," << pointer / x_res << endl;
#endif
           counter = 0;
           if (pointer < x_res * y_res && getPixel(pointer, 0, x_res, y_res, image) != lastEntry) {
               compressed.push_back(counter);
               lastEntry = getPixel(pointer+1, 0, x_res, y_res, image);
           }
       } else if (getPixel(pointer, 0, x_res, y_res, image) != lastEntry) {
           compressed.push_back(counter);
#if DEBUG == 1
           cout << (int) counter << " " << (int) lastEntry << " "<< pointer % x_res << "," << pointer / x_res << endl;
#endif
           counter = 0;
           lastEntry = getPixel(pointer, 0, x_res, y_res, image);
       }
    }
    compressed.push_back(++counter);
    return compressed;
}
