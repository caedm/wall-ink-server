#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include "letters.h"
#define X_RES0 640
#define Y_RES0 384
#define ASCII_OFFSET 32
#define LETTER_HEIGHT 13

using namespace std;

char image[X_RES0/8 * Y_RES0];

void setPixel(int x, int y, unsigned char color) {
    if (color == 0) {
        image[x/8 + X_RES0/8*y] = image[x/8 + X_RES0/8*y] & ((0x01 << (7-x%8)) ^ 0xff); //white
    }
    else if (color == 1) {
        image[x/8 + X_RES0/8*y] = image[x/8 + X_RES0/8*y] | (0x01 << (7-x%8)); //black
    }
    else if (color == 2) {
        image[x/8 + X_RES0/8*y] = image[x/8 + X_RES0/8*y] ^ (0x01 << (7-x%8)); //swap
    }
}

void drawRect(int x, int y, int width, int height, unsigned char color) {
    for (int x2 = 0; x2 < width; x2++) {
        for (int y2 = 0; y2 < height; y2++) {
            setPixel(x + x2, y + y2, color);
        }
    }
}

void initializeImage() {
    for (int i = 0; i < X_RES0/8 * Y_RES0; i++) {
        image[i] = 0;
    }
}

void drawCharacter(int x, int y, char c) {
   for (int i = 0; i < LETTER_HEIGHT; i++){
       image[x/8 + X_RES0/8*(y+LETTER_HEIGHT-1-i)] = image[x/8 + X_RES0/8*(y+LETTER_HEIGHT-1-i)] | (letters[c - ASCII_OFFSET][i] >> (x%8));
       if (x%8 > 0) {
           image[x/8 + X_RES0/8*(y+LETTER_HEIGHT-1-i) + 1] = image[x/8 + X_RES0/8*(y+LETTER_HEIGHT-1-i) + 1] | (letters[c - ASCII_OFFSET][i] << (8-x%8));
       }
   } 
}

void drawString(int x, int y, string str) {
    for (int i = 0; i < str.length(); i++) {
        drawCharacter(x + 9*i, y, str[i]);
    }
}

unsigned char reverseByte(unsigned char x) {
    static const unsigned char table[] = {
        0x00, 0x80, 0x40, 0xc0, 0x20, 0xa0, 0x60, 0xe0,
        0x10, 0x90, 0x50, 0xd0, 0x30, 0xb0, 0x70, 0xf0,
        0x08, 0x88, 0x48, 0xc8, 0x28, 0xa8, 0x68, 0xe8,
        0x18, 0x98, 0x58, 0xd8, 0x38, 0xb8, 0x78, 0xf8,
        0x04, 0x84, 0x44, 0xc4, 0x24, 0xa4, 0x64, 0xe4,
        0x14, 0x94, 0x54, 0xd4, 0x34, 0xb4, 0x74, 0xf4,
        0x0c, 0x8c, 0x4c, 0xcc, 0x2c, 0xac, 0x6c, 0xec,
        0x1c, 0x9c, 0x5c, 0xdc, 0x3c, 0xbc, 0x7c, 0xfc,
        0x02, 0x82, 0x42, 0xc2, 0x22, 0xa2, 0x62, 0xe2,
        0x12, 0x92, 0x52, 0xd2, 0x32, 0xb2, 0x72, 0xf2,
        0x0a, 0x8a, 0x4a, 0xca, 0x2a, 0xaa, 0x6a, 0xea,
        0x1a, 0x9a, 0x5a, 0xda, 0x3a, 0xba, 0x7a, 0xfa,
        0x06, 0x86, 0x46, 0xc6, 0x26, 0xa6, 0x66, 0xe6,
        0x16, 0x96, 0x56, 0xd6, 0x36, 0xb6, 0x76, 0xf6,
        0x0e, 0x8e, 0x4e, 0xce, 0x2e, 0xae, 0x6e, 0xee,
        0x1e, 0x9e, 0x5e, 0xde, 0x3e, 0xbe, 0x7e, 0xfe,
        0x01, 0x81, 0x41, 0xc1, 0x21, 0xa1, 0x61, 0xe1,
        0x11, 0x91, 0x51, 0xd1, 0x31, 0xb1, 0x71, 0xf1,
        0x09, 0x89, 0x49, 0xc9, 0x29, 0xa9, 0x69, 0xe9,
        0x19, 0x99, 0x59, 0xd9, 0x39, 0xb9, 0x79, 0xf9,
        0x05, 0x85, 0x45, 0xc5, 0x25, 0xa5, 0x65, 0xe5,
        0x15, 0x95, 0x55, 0xd5, 0x35, 0xb5, 0x75, 0xf5,
        0x0d, 0x8d, 0x4d, 0xcd, 0x2d, 0xad, 0x6d, 0xed,
        0x1d, 0x9d, 0x5d, 0xdd, 0x3d, 0xbd, 0x7d, 0xfd,
        0x03, 0x83, 0x43, 0xc3, 0x23, 0xa3, 0x63, 0xe3,
        0x13, 0x93, 0x53, 0xd3, 0x33, 0xb3, 0x73, 0xf3,
        0x0b, 0x8b, 0x4b, 0xcb, 0x2b, 0xab, 0x6b, 0xeb,
        0x1b, 0x9b, 0x5b, 0xdb, 0x3b, 0xbb, 0x7b, 0xfb,
        0x07, 0x87, 0x47, 0xc7, 0x27, 0xa7, 0x67, 0xe7,
        0x17, 0x97, 0x57, 0xd7, 0x37, 0xb7, 0x77, 0xf7,
        0x0f, 0x8f, 0x4f, 0xcf, 0x2f, 0xaf, 0x6f, 0xef,
        0x1f, 0x9f, 0x5f, 0xdf, 0x3f, 0xbf, 0x7f, 0xff,
    };
    return table[x];
}

void invert(){
    for (int i = 0; i < X_RES0/8 * Y_RES0; i++) {
        image[i] = image[i] ^ 0xff;
    }
}

void mirror() {
    char temp;
    for (int y = 0; y < Y_RES0; y++) {
        for (int x = 0; x < X_RES0/16; x++) {
            temp = image[y * X_RES0/8 + x]; 
            image[y * X_RES0/8 + x] = image[y * X_RES0/8 + X_RES0/8 - 1 - x];
            image[y * X_RES0/8 + X_RES0/8 - 1 - x] = temp;
        }
    }
    for (int i = 0; i < Y_RES0 * X_RES0/8; i++) {
        //image[i] = reverseByte(image[i]);
        temp = 0;
        for(int j=0;j<8;j++)
            temp |= ((image[i]>>j) & 1)<<(7-j);
        image[i] = temp;
    }
}

void drawImage(string roomName, string date, string time, string* reservations) {
    initializeImage();
    drawString(13,12,roomName + " Reservations");
    drawString(90,43,date);

    //Outer box
    drawRect(19,67,X_RES0 - 19*2, Y_RES0 - 67*2, 1);
    drawRect(24,72,X_RES0 - 24*2, Y_RES0 - 72*2, 0);

    drawString(31,Y_RES0 - 90, "Last updated " + date + ", " + time);
    drawString(84,Y_RES0 - 23, "reserve.et.byu.edu");

    //inner boxes
    drawRect(120,78,51,Y_RES0-78-101,1);
    drawRect(X_RES0-119,78,51,Y_RES0-78-101,1);

    //key
    drawRect(52,Y_RES0-59,51,28,1);
    drawRect(57,Y_RES0-55,41,20,0);
    drawString(111,Y_RES0-50,"Available");
    drawRect(211,Y_RES0-59,51,28,1);
    drawString(275,Y_RES0-50,"Reserved");


    invert();
    mirror();
}

int main(void) {
    //read from the database info
    ifstream fromDB;
    fromDB.open("fromDB");
    string mac_address;
    getline(fromDB, mac_address);
    string name;
    getline(fromDB, name);
    string dateNow;
    getline(fromDB, dateNow);
    string deviceType;
    getline(fromDB, deviceType);
    string reservations[] = {"Available","Available","Available","Available","Available","Available","Available","Available","Available","Available","Available","Available","Available","Available","Available","Available","Available","Available","Available","Available","Available","Available","Available","Available","Available","Available","Available","Available","Available","Available","Available"};
    string title;

    //Open the fromDB file and parse that info into the reservations array. Will probably need to be changed later to add more functionality.
    while (getline(fromDB, title)) {
        
        //Take in a date formatted string and decide which reservations[] time block it corresponds to
        string dateTimeStart;
        getline(fromDB, dateTimeStart);
        int startIndex;
        if (dateNow.compare(dateTimeStart.substr(0,10))) {
            int hour = atoi(dateTimeStart.substr(11,2).c_str());
            int minute = atoi(dateTimeStart.substr(14,2).c_str());
            hour -= 6;
            hour *= 2;
            minute /= 30;
            startIndex = hour + minute;
            if (startIndex < 0)
                startIndex = 0;
            if (startIndex > 30)
                startIndex = 30;
        } else {
            startIndex = 0;
        }
        
        //Take in a date formatted string and decide which reservations[] time block it corresponds to
        string dateTimeEnd;
        getline(fromDB, dateTimeEnd);
        int endIndex;
        if (dateNow.compare(dateTimeEnd.substr(0,10))) {
            int hour = atoi(dateTimeEnd.substr(11,2).c_str());
            int minute = atoi(dateTimeEnd.substr(14,2).c_str());
            hour -= 6;
            hour *= 2;
            minute /= 30;
            endIndex = hour + minute;
            if (endIndex < 0)
                endIndex = 0;
            if (endIndex > 31)
                endIndex = 31;
        } else {
            endIndex = 31;
        }

        for (int i = startIndex; i < endIndex; i++) {
            reservations[i] = title;
        }
    }

    //actually generate the desired image
    drawImage(name, dateNow, "05:38pm", reservations);

    //write to a file
    ofstream(mac_address, ios::binary).write(image, X_RES0/8 * Y_RES0);
    return 0;
}
