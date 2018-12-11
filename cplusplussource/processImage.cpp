#include <iostream>
#include "processImage.h"
#include "sha1.h"
#include <bitset>
#include <cstring>

#ifndef IMAGE_KEY
//this is just the default image key. You should be changing this in the config file instead.
#define IMAGE_KEY "hunter2"
#endif

uint8_t getPixel(unsigned long int x, unsigned long int y, uint16_t x_res, uint16_t y_res, uint8_t* image) {
    return (image[x/8 + x_res*y/8] >> (7 - x%8)) & 0x01;
}

std::vector<uint8_t> processImage(uint8_t* image, uint32_t sleepTime, uint16_t x_res, uint16_t y_res, std::string mac_address) {
    std::vector<uint8_t> processed;
    processed.clear();
    time_t currentTime = time(nullptr);
    uint8_t* processedTime = (uint8_t*) malloc(4);
    uint8_t* nextTime = (uint8_t*) malloc(4);
    uint8_t* time = (uint8_t*) malloc(8);
    *((uint32_t*) processedTime) = currentTime;
    *((uint32_t*) nextTime) = *((uint32_t*) processedTime) + sleepTime;
    memcpy(time, processedTime, 4);
    memcpy(time+4, nextTime, 4);
#if DEBUG == 1
    cout << hex;
    cout << "time size: " << sizeof(currentTime) << endl << "current time: " << currentTime << endl;
    cout << "processed time: " << *((uint32_t*) processedTime) << endl;
    cout << "next time: " << *((uint32_t*) nextTime) << endl;
    cout << "byte by byte: " << +processedTime[0] << " " << +processedTime[1] << " " << +processedTime[2] << " " << +processedTime[3] << endl;
#endif
    uint8_t hash[80];
    SHA1Context sha;
    int err;
    err = SHA1Reset(&sha);
    err = SHA1Input(&sha, time, 8);
    err = SHA1Result(&sha, hash);
    err = SHA1Reset(&sha);
    err = SHA1Input(&sha, (uint8_t*) mac_address.c_str(), 12);
    err = SHA1Result(&sha, hash+20);
    char imageKey[] = IMAGE_KEY;
    err = SHA1Reset(&sha);
    err = SHA1Input(&sha, (uint8_t*) imageKey, strlen(imageKey));
    err = SHA1Result(&sha, hash+40);
    uint8_t finalTimeHash[20];
    err = SHA1Reset(&sha);
    err = SHA1Input(&sha, (uint8_t*) hash, 60);
    err = SHA1Result(&sha, finalTimeHash);
    memcpy(hash+60, hash+40, 20);
    for (int i = 0; i < 20; i++)
        processed.push_back(finalTimeHash[i]);
    processed.push_back(processedTime[0]);
    processed.push_back(processedTime[1]);
    processed.push_back(processedTime[2]);
    processed.push_back(processedTime[3]);
    processed.push_back(nextTime[0]);
    processed.push_back(nextTime[1]);
    processed.push_back(nextTime[2]);
    processed.push_back(nextTime[3]);
    //generate image hash
    err = SHA1Reset(&sha);
    err = SHA1Input(&sha, image, x_res*y_res/8);
    err = SHA1Result(&sha, hash+40);
    uint8_t finalHash[20];
    err = SHA1Reset(&sha);
    err = SHA1Input(&sha, (uint8_t*) hash + 20, 60);
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
            printf("%02X ", (hash+40)[i]);
        }
        printf("\n");
    }
    for (int i = 0; i < 20; i++)
        processed.push_back(finalHash[i]);

    uint8_t hashOfEverything[20];
    err = SHA1Reset(&sha);
    err = SHA1Input(&sha, (uint8_t*) hash, 80);
    err = SHA1Result(&sha, hashOfEverything);
    for (int i = 0; i < 20; i++)
        processed.push_back(hashOfEverything[i]);
    free(processedTime);
    free(nextTime);
    for (uint32_t pointer = 0; pointer < x_res* y_res/8; pointer++) {
        processed.push_back(image[pointer]);
    }
    return processed;
}
