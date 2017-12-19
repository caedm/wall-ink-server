#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <vector>
#include <ctime>
#include "letters.h"
#include "Adafruit-GFX-Library/Adafruit_GFX.cpp"
#include "Adafruit-GFX-Library/Fonts/FreeMono12pt7b.h"
#include "Adafruit-GFX-Library/Fonts/FreeMono18pt7b.h"
#include "Adafruit-GFX-Library/Fonts/FreeMono24pt7b.h"
#include "Adafruit-GFX-Library/Fonts/FreeMono9pt7b.h"
#include "Adafruit-GFX-Library/Fonts/FreeMonoBold12pt7b.h"
#include "Adafruit-GFX-Library/Fonts/FreeMonoBold18pt7b.h"
#include "Adafruit-GFX-Library/Fonts/FreeMonoBold24pt7b.h"
#include "Adafruit-GFX-Library/Fonts/FreeMonoBold9pt7b.h"
#include "Adafruit-GFX-Library/Fonts/FreeMonoBoldOblique12pt7b.h"
#include "Adafruit-GFX-Library/Fonts/FreeMonoBoldOblique18pt7b.h"
#include "Adafruit-GFX-Library/Fonts/FreeMonoBoldOblique24pt7b.h"
#include "Adafruit-GFX-Library/Fonts/FreeMonoBoldOblique9pt7b.h"
#include "Adafruit-GFX-Library/Fonts/FreeMonoOblique12pt7b.h"
#include "Adafruit-GFX-Library/Fonts/FreeMonoOblique18pt7b.h"
#include "Adafruit-GFX-Library/Fonts/FreeMonoOblique24pt7b.h"
#include "Adafruit-GFX-Library/Fonts/FreeMonoOblique9pt7b.h"
#include "Adafruit-GFX-Library/Fonts/FreeSans12pt7b.h"
#include "Adafruit-GFX-Library/Fonts/FreeSans18pt7b.h"
#include "Adafruit-GFX-Library/Fonts/FreeSans24pt7b.h"
#include "Adafruit-GFX-Library/Fonts/FreeSans9pt7b.h"
#include "Adafruit-GFX-Library/Fonts/FreeSansBold12pt7b.h"
#include "Adafruit-GFX-Library/Fonts/FreeSansBold18pt7b.h"
#include "Adafruit-GFX-Library/Fonts/FreeSansBold24pt7b.h"
#include "Adafruit-GFX-Library/Fonts/FreeSansBold9pt7b.h"
#include "Adafruit-GFX-Library/Fonts/FreeSansBoldOblique12pt7b.h"
#include "Adafruit-GFX-Library/Fonts/FreeSansBoldOblique18pt7b.h"
#include "Adafruit-GFX-Library/Fonts/FreeSansBoldOblique24pt7b.h"
#include "Adafruit-GFX-Library/Fonts/FreeSansBoldOblique9pt7b.h"
#include "Adafruit-GFX-Library/Fonts/FreeSansOblique12pt7b.h"
#include "Adafruit-GFX-Library/Fonts/FreeSansOblique18pt7b.h"
#include "Adafruit-GFX-Library/Fonts/FreeSansOblique24pt7b.h"
#include "Adafruit-GFX-Library/Fonts/FreeSansOblique9pt7b.h"
#include "Adafruit-GFX-Library/Fonts/FreeSerif12pt7b.h"
#include "Adafruit-GFX-Library/Fonts/FreeSerif18pt7b.h"
#include "Adafruit-GFX-Library/Fonts/FreeSerif24pt7b.h"
#include "Adafruit-GFX-Library/Fonts/FreeSerif9pt7b.h"
#include "Adafruit-GFX-Library/Fonts/FreeSerifBold12pt7b.h"
#include "Adafruit-GFX-Library/Fonts/FreeSerifBold18pt7b.h"
#include "Adafruit-GFX-Library/Fonts/FreeSerifBold24pt7b.h"
#include "Adafruit-GFX-Library/Fonts/FreeSerifBold9pt7b.h"
#include "Adafruit-GFX-Library/Fonts/FreeSerifBoldItalic12pt7b.h"
#include "Adafruit-GFX-Library/Fonts/FreeSerifBoldItalic18pt7b.h"
#include "Adafruit-GFX-Library/Fonts/FreeSerifBoldItalic24pt7b.h"
#include "Adafruit-GFX-Library/Fonts/FreeSerifBoldItalic9pt7b.h"
#include "Adafruit-GFX-Library/Fonts/FreeSerifItalic12pt7b.h"
#include "Adafruit-GFX-Library/Fonts/FreeSerifItalic18pt7b.h"
#include "Adafruit-GFX-Library/Fonts/FreeSerifItalic24pt7b.h"
#include "Adafruit-GFX-Library/Fonts/FreeSerifItalic9pt7b.h"
#include "Adafruit-GFX-Library/Fonts/Org_01.h"
#include "Adafruit-GFX-Library/Fonts/Picopixel.h"
#include "Adafruit-GFX-Library/Fonts/Tiny3x3a2pt7b"
#include "Adafruit-GFX-Library/Fonts/TomThumb.h"
#define X_RES0 384
#define Y_RES0 640
#define X_RES1 400
#define Y_RES1 300
#define X_RES2 640
#define Y_RES2 384
#define X_RES3 640
#define Y_RES3 384
#define ASCII_OFFSET 32
#define LETTER_HEIGHT 13
#define DEBUG 0

using namespace std;

uint8_t* image;
GFXcanvas1* canvas;
uint32_t sleepTime;

uint16_t x_res;
uint16_t y_res;

uint8_t getPixel(unsigned long int x, unsigned long int y) {
    return (image[x/8 + x_res*y/8] >> (7 - x%8)) & 0x01;
}

vector<uint8_t> compressImage() {
    vector<uint8_t> compressed;
    compressed.clear();
    time_t currentTime = time(nullptr);
    uint8_t* compressedTime = (uint8_t*) malloc(4);
    uint8_t* nextTime = (uint8_t*) malloc(4);
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

/*
vector<unsigned char> compressImage() {
    vector<unsigned char> compressed;
    compressed.clear();
    for (int y = 0; y < y_res; y++) {
        int switches = 0;
        for (int x = 1; x < x_res; x++)
            if (getPixel(x,y) != getPixel(x-1,y))
                ++switches;
        if (switches >= y_res/8) {
            compressed.push_back(0);
            for (int x = 0; x < x_res/8; x++) {
                compressed.push_back(image[x + y*x_res/8]);
            }
        } else {
            if (getPixel(0,y) == 1)
                compressed.push_back(switches);
            else
                compressed.push_back(switches);
            unsigned char counter = 1;
            for (int x = 1; x < x_res; x++) {
                if (getPixel(x,y) != getPixel(x-1,y)) {
                    compressed.push_back(counter);
                    counter = 0;
                } else if (counter == 255) {
                    counter = 0;
                    compressed.push_back(0);
                }
                ++counter;
            }
            compressed.push_back(counter);
        }
    }
    
    return compressed;
}
*/

void setPixel(int x, int y, unsigned char color) {
    if (color == 0) {
        image[x/8 + x_res/8*y] = image[x/8 + x_res/8*y] & ((0x01 << (7-x%8)) ^ 0xff); //white
    }
    else if (color == 1) {
        image[x/8 + x_res/8*y] = image[x/8 + x_res/8*y] | (0x01 << (7-x%8)); //black
    }
    else if (color == 2) {
        image[x/8 + x_res/8*y] = image[x/8 + x_res/8*y] ^ (0x01 << (7-x%8)); //swap
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
    image = (uint8_t*) malloc((x_res * y_res)/8);
    for (int i = 0; i < x_res * y_res/8; i++) {
        image[i] = 0;
    }
}

void drawCharacter(int x, int y, char c) {
   for (int i = 0; i < LETTER_HEIGHT; i++){
       image[x/8 + x_res/8*(y+LETTER_HEIGHT-1-i)] = image[x/8 + x_res/8*(y+LETTER_HEIGHT-1-i)] | (letters[c - ASCII_OFFSET][i] >> (x%8));
       if (x%8 > 0) {
           image[x/8 + x_res/8*(y+LETTER_HEIGHT-1-i) + 1] = image[x/8 + x_res/8*(y+LETTER_HEIGHT-1-i) + 1] | (letters[c - ASCII_OFFSET][i] << (8-x%8));
       }
   } 
}

void drawString(int x, int y, string str) {
    for (int i = 0; i < str.length(); i++) {
        drawCharacter(x + 9*i, y, str[i]);
    }
}

void drawFancyString(string str, int16_t x, int16_t y) {
    canvas->setCursor(x, y);
    for (int i = 0; i < str.length(); i++)
        canvas->write(str[i]);
}

bool drawCenteredString(string str, int16_t y){
    int16_t x1, y1;
    uint16_t w, h;
    canvas->getTextBounds(str.c_str(), 0, y, &x1, &y1, &w, &h);
    if (w <= x_res)
    	drawFancyString(str, (x_res-w)/2, y);
    return w <= x_res;
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
    for (int i = 0; i < x_res/8 * y_res; i++) {
        image[i] = image[i] ^ 0xff;
    }
}

//not sure that this will work
void rotate() {
    GFXcanvas1* c = new GFXcanvas1(y_res, x_res);
    for (int x = 0; x < x_res; x++) {
        for (int y = 0; y < y_res; y++) {
            c->drawPixel(y, x, getPixel(x, y));
        }
    }
    delete canvas;
    canvas = c;
    int16_t temp = x_res;
    x_res = y_res;
    y_res = temp;
}

void mirror() {
    char temp;
    for (int y = 0; y < y_res; y++) {
        for (int x = 0; x < x_res/16; x++) {
            temp = image[y * x_res/8 + x]; 
            image[y * x_res/8 + x] = image[y * x_res/8 + x_res/8 - 1 - x];
            image[y * x_res/8 + x_res/8 - 1 - x] = temp;
        }
    }
    for (int i = 0; i < y_res * x_res/8; i++) {
        //image[i] = reverseByte(image[i]);
        temp = 0;
        for(int j=0;j<8;j++)
            temp |= ((image[i]>>j) & 1)<<(7-j);
        image[i] = temp;
    }
}

string reservationBlockToTime(int block) {
    stringstream hourStream;
    hourStream << block/2 + 6;
    string hour = hourStream.str();
    if (hour.length() < 2) {
        hour = "0" + hour;
    }
    stringstream minuteStream;
    minuteStream << (block % 2) * 30;
    string minute = minuteStream.str();
    if (minute.length() < 2) {
        minute += "0";
    }
    return hour + ":" + minute;
}

string militaryTimeToNormalPersonTime(string military) {
    int hour = atoi(military.substr(0,2).c_str());
    string ampm = "am";
    if (hour > 12) {
        hour -= 12;
        ampm = "pm";
    }
    if (hour == 0)
        hour = 12;
    stringstream hourStream;
    hourStream << hour;
    return hourStream.str() + ":" + military.substr(3,2) + ampm;
}

string fancyDateFromYYYY_MM_DD(string YYYY_MM_DD) {
    int year = atoi(YYYY_MM_DD.substr(0,4).c_str());
    int month = atoi(YYYY_MM_DD.substr(5,2).c_str());
    int day = atoi(YYYY_MM_DD.substr(8,2).c_str());
    string months[12] = {"January",
                         "February",
                         "March",
                         "April",
                         "May",
                         "June",
                         "July",
                         "August",
                         "September",
                         "October",
                         "November",
                         "December"};
    string monthName = months[month-1];
    stringstream fancyDate;
    fancyDate << monthName << " " << day << ", " << year;
    return fancyDate.str();
}

struct reservation {
    public:
        string title;
        int startBlock;
        int endBlock;
};

vector<reservation> parseReservations(string* reservations) {
    vector<reservation> reservs;
    reservs.clear();
    int startBlock = 0;
    string title = reservations[0];
    for (int block = 1; block < 32; block++) {
        if (title.compare(reservations[block]) != 0) {
            reservation r;
            r.title = title;
            r.startBlock = startBlock;
            r.endBlock = block;
            reservs.push_back(r);
            title = reservations[block];
            startBlock = block;
        }
    }
    reservation r;
    r.title = title;
    r.startBlock = startBlock;
    r.endBlock = 32;
    reservs.push_back(r);
    return reservs;
}

void setSleepTime(uint32_t increment) { //increment is the target number of seconds between refreshes
    time_t currentTimeTemp = time(nullptr);
    uint32_t currentTime = currentTimeTemp;
    if (currentTime % 86400 > 77400) { //if time is past 9:30pm, wake at 6:30am
        sleepTime = currentTime - (currentTime % 86400) + 86400 + 23400;
    } else {
        sleepTime = increment - (currentTime % increment) + increment/32;
    }
}

void drawImage0(string roomName, string date, string time, string* reservations) { //portrait 7"
    //set sleepTime
    setSleepTime(900);

    //Draw room name
    canvas->setFont(&FreeSans18pt7b);
    canvas->setTextColor(1);
    canvas->setTextWrap(false);
    if (!drawCenteredString(roomName + " Reservations", 32)) {
        if (!drawCenteredString(roomName, 32)) {
            canvas->setFont(&FreeSans12pt7b);
            if (!drawCenteredString(roomName + " Reservations", 29)) {
                if (!drawCenteredString(roomName, 29)) {
                    canvas->setFont(&FreeSans9pt7b);
                    if (!drawCenteredString(roomName + " Reservations", 28)) {
                        if (!drawCenteredString(roomName, 28)) {
                        }
                    }
                }
            }
        }
    }
    canvas->setFont(&FreeSans9pt7b);

    //Draw Date
    drawCenteredString(fancyDateFromYYYY_MM_DD(date), 53);

    //Outer box
    drawRect(19,67,x_res - 19*2, y_res - 67*2, 1);
    drawRect(24,72,x_res - 24*2, y_res - 72*2, 0);

    drawCenteredString("Last updated " + militaryTimeToNormalPersonTime(time), y_res-80);
    drawCenteredString("reserve.et.byu.edu", y_res-13);

    uint16_t boxCoordinates[32][2] = {
        {50,78+29*0},
        {50,78+29*1},
        {50,78+29*2},
        {50,78+29*3},
        {50,78+29*4},
        {50,78+29*5},
        {50,78+29*6},
        {50,78+29*7},
        {50,78+29*8},
        {50,78+29*9},
        {50,78+29*10},
        {50,78+29*11},
        {50,78+29*12},
        {50,78+29*13},
        {50,78+29*14},
        {50,78+29*15},
        {195,78+29*0},
        {195,78+29*1},
        {195,78+29*2},
        {195,78+29*3},
        {195,78+29*4},
        {195,78+29*5},
        {195,78+29*6},
        {195,78+29*7},
        {195,78+29*8},
        {195,78+29*9},
        {195,78+29*10},
        {195,78+29*11},
        {195,78+29*12},
        {195,78+29*13},
        {195,78+29*14},
        {195,78+29*15}
    };

    //For each time
    for (int i = 0; i < 32; i++) {

        //generate and display time string
        ostringstream time;
        string ampm = "am";
        int hour = i + 12;
        hour /= 2;
        if (hour > 11)
            ampm = "pm";
        if (hour > 12)
            hour -= 12;
        if (hour < 10)
            time << " ";
        time << hour;
        time << ":";
        time << (i % 2) * 30;
        if (i % 2 == 0)
            time << "0";
        time << ampm;
        drawString(boxCoordinates[i][0], boxCoordinates[i][1]+8, time.str()); 

        //draw black boxes
        drawRect(boxCoordinates[i][0]+70, boxCoordinates[i][1], 51, 29, 1);

        //draw white boxes for open time slots
        if (reservations[i].compare("Available") == 0)
            drawRect(boxCoordinates[i][0]+75, boxCoordinates[i][1]+4, 41, 22, 0);
    }
    //finish top and bottom of the boxes we just drew
    drawRect(120, 77, 51, 1, 1);
    drawRect(265, 77, 51, 1, 1);
    drawRect(120, 542, 51, 2, 1);
    drawRect(265, 542, 51, 2, 1);

    //key
    drawRect(52,y_res-59,51,28,1);
    drawRect(57,y_res-55,41,20,0);
    drawString(111,y_res-50,"Available");
    drawRect(211,y_res-59,51,28,1);
    drawString(275,y_res-50,"Reserved");

    invert();
    //mirror();
}

void drawImage1(string roomName, string date, string time, string* reservations) { //landscape 4", shows 2 appointments
    //set sleepTime
    setSleepTime(900);

    //Draw room name and date
    canvas->setFont(&FreeSansBold12pt7b);
    canvas->setTextColor(1);
    canvas->setTextWrap(false);
    drawCenteredString(roomName, 26);
    drawCenteredString(fancyDateFromYYYY_MM_DD(date), 53);

    //Draw dividing line
    drawRect(0,62,x_res,2,1);

    //Get current block
    int currentBlock;
    currentBlock = (atoi(time.substr(0,2).c_str()) - 6) * 2;
    currentBlock += atoi(time.substr(3,2).c_str()) / 30;

    //Get current event
    string currentTitle = reservations[currentBlock];
    string currentStart = reservationBlockToTime(currentBlock);
    int blockNextStart = currentBlock;
    while (currentTitle.compare(reservations[++blockNextStart]) == 0 && blockNextStart < 32) {}
    string currentEnd = reservationBlockToTime(blockNextStart);
    
    //Draw current event
    string currentEventTime = militaryTimeToNormalPersonTime(currentStart) + " - " + militaryTimeToNormalPersonTime(currentEnd);
    canvas->setFont(&FreeSansBold18pt7b);
    drawFancyString(currentEventTime, 8, 100);
    canvas->setFont(&FreeSans18pt7b);
    canvas->setTextWrap(true);
    drawFancyString(currentTitle, 8, 140);
    canvas->setTextWrap(false);

    //Get next event
    if (blockNextStart < 31) {
		string nextTitle = reservations[blockNextStart];
		string nextStart = reservationBlockToTime(blockNextStart);
		int blockNextEnd = blockNextStart;
		while (nextTitle.compare(reservations[++blockNextEnd]) == 0 && blockNextEnd < 32) {}
		string nextEnd = reservationBlockToTime(blockNextEnd);

		//Draw next event
		string nextEventTime = militaryTimeToNormalPersonTime(nextStart) + " - " + militaryTimeToNormalPersonTime(nextEnd);
		canvas->setFont(&FreeSansBold12pt7b);
		drawFancyString(nextEventTime, 9, 230);
		canvas->setFont(&FreeSans12pt7b);
        canvas->setTextWrap(true);
		drawFancyString(nextTitle, 8, 260);
        canvas->setTextWrap(false);
    }


    invert();
}

void drawImage2(string roomName, string date, string time, string* reservations) { //7" landscape, shows 2 appointments plus blocks
    //set sleepTime
    setSleepTime(900);

    canvas->setFont(&FreeSans24pt7b);
    canvas->setTextColor(1);
    canvas->setTextWrap(false);

    //draw room name
    if (!drawCenteredString(roomName + " Reservations", 45)) {
        if (!drawCenteredString(roomName, 45)) {
            canvas->setFont(&FreeSans18pt7b);
            if (!drawCenteredString(roomName + " Reservations", 42)) {
                if (!drawCenteredString(roomName, 42)) {
                    canvas->setFont(&FreeSans12pt7b);
                    if (!drawCenteredString(roomName + " Reservations", 39)) {
                        if (!drawCenteredString(roomName, 39)) {
                        }
                    }
                }
            }
        }
    }
    
    //draw date
    canvas->setFont(&FreeSans18pt7b);
    drawCenteredString(fancyDateFromYYYY_MM_DD(date), 85);

    //draw line under date
    drawRect(0,95,x_res,1,1);

    //Get current block
    int currentBlock;
    currentBlock = (atoi(time.substr(0,2).c_str()) - 6) * 2;
    currentBlock += atoi(time.substr(3,2).c_str()) / 30;

    //Get current event
    string currentTitle = reservations[currentBlock];
    string currentStart = reservationBlockToTime(currentBlock);
    int blockNextStart = currentBlock;
    while (currentTitle.compare(reservations[++blockNextStart]) == 0 && blockNextStart < 32) {}
    string currentEnd = reservationBlockToTime(blockNextStart);
    
    //Draw current event
    string currentEventTime = militaryTimeToNormalPersonTime(currentStart) + " - " + militaryTimeToNormalPersonTime(currentEnd);
    canvas->setFont(&FreeSansBold18pt7b);
    drawFancyString(currentEventTime, 8, 130);
    canvas->setFont(&FreeSans18pt7b);
    canvas->setTextWrap(true);
    drawFancyString(currentTitle, 8, 170);
    canvas->setTextWrap(false);

    //Get next event
    if (blockNextStart < 31) {
		string nextTitle = reservations[blockNextStart];
		string nextStart = reservationBlockToTime(blockNextStart);
		int blockNextEnd = blockNextStart;
		while (nextTitle.compare(reservations[++blockNextEnd]) == 0 && blockNextEnd < 32) {}
		string nextEnd = reservationBlockToTime(blockNextEnd);

		//Draw next event
		string nextEventTime = militaryTimeToNormalPersonTime(nextStart) + " - " + militaryTimeToNormalPersonTime(nextEnd);
		canvas->setFont(&FreeSansBold12pt7b);
		drawFancyString(nextEventTime, 9, 250);
		canvas->setFont(&FreeSans12pt7b);
        canvas->setTextWrap(true);
		drawFancyString(nextTitle, 8, 280);
        canvas->setTextWrap(false);
    }

    //draw times
    canvas->setFont(&FreeSansBold9pt7b);
    for (int hour = 7; hour < 22; hour++) {
        stringstream hourString;
        if (hour < 13)
            hourString << hour;
        else
            hourString << hour-12;
        drawFancyString(hourString.str(), hour*40 - 249, 380);
    }

    //draw blocks
    for (int block = 0; block < 32; block++) {
        drawRect(block*20, 322, 20, 37, 1);
        if (reservations[block].compare("Available") == 0) {
            drawRect(block*20 + 1, 324, 18, 33, 0);
        }
    }

    //trim blocks
    for (int block = 0; block < 32; block++) {
        if (reservations[block].compare("Available") == 0) {
            if (block > 0) {
                //round corners if edge case
                if (reservations[block-1].compare("Available") != 0) {
                    drawRect(block*20 - 4, 322, 5, 2, 0);
                    drawRect(block*20, 323, 1, 1, 1);
                    drawRect(block*20 - 4, 323, 1, 1, 1);
                    drawRect(block*20 - 4, 357, 5, 2, 0);
                    drawRect(block*20, 357, 1, 1, 1);
                    drawRect(block*20 - 4, 357, 1, 1, 1);
                    drawRect(block*20 - 2, 322, 1, 37, 0);
                }
            }
            if (block < 31) {
                //round corners if edge case
                if (reservations[block+1].compare("Available") != 0) {
                    drawRect(block*20 + 19, 322, 5, 2, 0);
                    drawRect(block*20 + 19, 323, 1, 1, 1);
                    drawRect(block*20 + 23, 323, 1, 1, 1);
                    drawRect(block*20 + 19, 357, 5, 2, 0);
                    drawRect(block*20 + 19, 357, 1, 1, 1);
                    drawRect(block*20 + 23, 357, 1, 1, 1);
                    drawRect(block*20 + 21, 322, 1, 37, 0);
                }
            }
        } else {
            if (block > 0) {
                //round corners if edge case
                if (reservations[block-1].compare(reservations[block]) != 0 && reservations[block-1].compare("Available") != 0) {
                    drawRect(block*20 - 2, 322, 5, 2, 0);
                    drawRect(block*20 + 2, 323, 1, 1, 1);
                    drawRect(block*20 - 2, 323, 1, 1, 1);
                    drawRect(block*20 - 2, 357, 5, 2, 0);
                    drawRect(block*20 + 2, 357, 1, 1, 1);
                    drawRect(block*20 - 2, 357, 1, 1, 1);
                    drawRect(block*20, 322, 1, 37, 0);
                }
            }
        }
    }

    //draw arrow
    drawRect(currentBlock*20 - 1, 320, 2, 1, 1);
    drawRect(currentBlock*20 - 2, 319, 4, 1, 1);
    drawRect(currentBlock*20 - 3, 318, 6, 1, 1);
    drawRect(currentBlock*20 - 4, 316, 8, 2, 1);

    //draw time above the arrow
    canvas->setFont(&FreeSansBold9pt7b);
    drawFancyString(militaryTimeToNormalPersonTime(currentStart), currentBlock*20 - 30, 310);


    invert();
}

void drawImage3(string roomName, string date, string time, string* reservations) { //7" landscape, shows 3 appointments plus blocks
    //set sleepTime
    setSleepTime(1800);

    canvas->setFont(&FreeSans24pt7b);
    canvas->setTextColor(1);
    canvas->setTextWrap(false);

    //parse reservations
    vector<reservation> reservs = parseReservations(reservations);

    //draw room name
    if (!drawCenteredString(roomName + " Reservations", 45)) {
        if (!drawCenteredString(roomName, 45)) {
            canvas->setFont(&FreeSans18pt7b);
            if (!drawCenteredString(roomName + " Reservations", 42)) {
                if (!drawCenteredString(roomName, 42)) {
                    canvas->setFont(&FreeSans12pt7b);
                    if (!drawCenteredString(roomName + " Reservations", 39)) {
                        if (!drawCenteredString(roomName, 39)) {
                        }
                    }
                }
            }
        }
    }
    
    //draw date
    canvas->setFont(&FreeSans18pt7b);
    drawCenteredString(fancyDateFromYYYY_MM_DD(date), 85);

    //draw line under date
    drawRect(0,95,x_res,1,1);

    //Get current block
    int currentBlock;
    currentBlock = (atoi(time.substr(0,2).c_str()) - 6) * 2;
    currentBlock += atoi(time.substr(3,2).c_str()) / 30;

    //Get current event
    int currentEventIndex;
    for (int i = 0; i < reservs.size(); i++) {
        if (currentBlock >= reservs.at(i).startBlock && currentBlock <= reservs.at(i).endBlock)
            currentEventIndex = i;
    }
    if (reservs.size() > 2 && currentEventIndex == 0) {
        currentEventIndex++;
    } else if (reservs.size() > 2 && currentEventIndex == reservs.size() - 1) {
        currentEventIndex--;
    }
    
    //Draw previous event
    if (currentEventIndex > 0) {
        string prevEventTime = militaryTimeToNormalPersonTime(reservationBlockToTime(reservs.at(currentEventIndex-1).startBlock)) + " - " + militaryTimeToNormalPersonTime(reservationBlockToTime(reservs.at(currentEventIndex-1).endBlock));
		canvas->setFont(&FreeSansBold12pt7b);
		drawFancyString(prevEventTime, 9, 125);
		canvas->setFont(&FreeSans12pt7b);
        canvas->setTextWrap(true);
		drawFancyString(reservs.at(currentEventIndex-1).title, 8, 155);
        canvas->setTextWrap(false);
    }

    //Draw current event
    string currentEventTime = militaryTimeToNormalPersonTime(reservationBlockToTime(reservs.at(currentEventIndex).startBlock)) + " - " + militaryTimeToNormalPersonTime(reservationBlockToTime(reservs.at(currentEventIndex).endBlock));
    canvas->setFont(&FreeSansBold12pt7b);
    drawFancyString(currentEventTime, 8, 200);
    canvas->setFont(&FreeSans12pt7b);
    canvas->setTextWrap(true);
    drawFancyString(reservs.at(currentEventIndex).title, 8, 230);
    canvas->setTextWrap(false);

    //Draw next event
    if (reservs.size() > currentEventIndex+1) {
        string nextEventTime = militaryTimeToNormalPersonTime(reservationBlockToTime(reservs.at(currentEventIndex+1).startBlock)) + " - " + militaryTimeToNormalPersonTime(reservationBlockToTime(reservs.at(currentEventIndex+1).endBlock));
		canvas->setFont(&FreeSansBold12pt7b);
		drawFancyString(nextEventTime, 9, 275);
		canvas->setFont(&FreeSans12pt7b);
        canvas->setTextWrap(true);
		drawFancyString(reservs.at(currentEventIndex+1).title, 8, 305);
        canvas->setTextWrap(false);
    }

    //draw times
    canvas->setFont(&FreeSansBold9pt7b);
    for (int hour = 7; hour < 22; hour++) {
        stringstream hourString;
        if (hour < 13)
            hourString << hour;
        else
            hourString << hour-12;
        drawFancyString(hourString.str(), hour*40 - 249, 380);
    }

    //draw blocks
    for (int block = 0; block < 32; block++) {
        drawRect(block*20, 322, 20, 37, 1);
        if (reservations[block].compare("Available") == 0) {
            drawRect(block*20 + 1, 324, 18, 33, 0);
        }
    }

    //trim blocks
    for (int block = 0; block < 32; block++) {
        if (reservations[block].compare("Available") == 0) {
            if (block > 0) {
                //round corners if edge case
                if (reservations[block-1].compare("Available") != 0) {
                    drawRect(block*20 - 4, 322, 5, 2, 0);
                    drawRect(block*20, 323, 1, 1, 1);
                    drawRect(block*20 - 4, 323, 1, 1, 1);
                    drawRect(block*20 - 4, 357, 5, 2, 0);
                    drawRect(block*20, 357, 1, 1, 1);
                    drawRect(block*20 - 4, 357, 1, 1, 1);
                    drawRect(block*20 - 2, 322, 1, 37, 0);
                }
            }
            if (block < 31) {
                //round corners if edge case
                if (reservations[block+1].compare("Available") != 0) {
                    drawRect(block*20 + 19, 322, 5, 2, 0);
                    drawRect(block*20 + 19, 323, 1, 1, 1);
                    drawRect(block*20 + 23, 323, 1, 1, 1);
                    drawRect(block*20 + 19, 357, 5, 2, 0);
                    drawRect(block*20 + 19, 357, 1, 1, 1);
                    drawRect(block*20 + 23, 357, 1, 1, 1);
                    drawRect(block*20 + 21, 322, 1, 37, 0);
                }
            }
        } else {
            if (block > 0) {
                //round corners if edge case
                if (reservations[block-1].compare(reservations[block]) != 0 && reservations[block-1].compare("Available") != 0) {
                    drawRect(block*20 - 2, 322, 5, 2, 0);
                    drawRect(block*20 + 2, 323, 1, 1, 1);
                    drawRect(block*20 - 2, 323, 1, 1, 1);
                    drawRect(block*20 - 2, 357, 5, 2, 0);
                    drawRect(block*20 + 2, 357, 1, 1, 1);
                    drawRect(block*20 - 2, 357, 1, 1, 1);
                    drawRect(block*20, 322, 1, 37, 0);
                }
            }
        }
    }

    //draw arrow
    drawRect(currentBlock*20 - 1, 320, 2, 1, 1);
    drawRect(currentBlock*20 - 2, 319, 4, 1, 1);
    drawRect(currentBlock*20 - 3, 318, 6, 1, 1);
    drawRect(currentBlock*20 - 4, 316, 8, 2, 1);

    //draw time above the arrow
    //canvas->setFont(&FreeSansBold9pt7b);
    //drawFancyString(militaryTimeToNormalPersonTime(currentStart), currentBlock*20 - 30, 310);


    invert();
}

int main(int argc, char* argv[]) {
    //read from the database info
    ifstream fromDB;
    if (argc == 1)
        fromDB.open("fromDB");
    else
        fromDB.open(argv[1]);
    string mac_address;
    getline(fromDB, mac_address);
    string name;
    getline(fromDB, name);
    string dateNow;
    getline(fromDB, dateNow);
    string timeNow;
    getline(fromDB, timeNow);
    string deviceType;
    getline(fromDB, deviceType);

    if (deviceType.compare("0") == 0) {
        x_res = X_RES0;
        y_res = Y_RES0;
    } else if (deviceType.compare("1") == 0) {
        x_res = X_RES1;
        y_res = Y_RES1;
    } else if (deviceType.compare("2") == 0) {
        x_res = X_RES2;
        y_res = Y_RES2;
    } else if (deviceType.compare("3") == 0) {
        x_res = X_RES3;
        y_res = Y_RES3;
    }
    canvas = new GFXcanvas1(x_res, y_res);
    canvas->fillScreen(0);
    image = canvas->getBuffer();

    string reservations[32];
    for (int i = 0; i < 32; i++) {
        reservations[i] = "Available";
    }
    string title;

    //Open the fromDB file and parse that info into the reservations array. Will probably need to be changed later to add more functionality.
    while (getline(fromDB, title)) {
        
        //Take in a date formatted string and decide which reservations[] time block it corresponds to
        string dateTimeStart;
        getline(fromDB, dateTimeStart);
        int startIndex;
        if (dateNow.compare(dateTimeStart.substr(0,10)) == 0) {
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
        if (dateNow.compare(dateTimeEnd.substr(0,10)) == 0) {
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
    if (deviceType.compare("0") == 0) {
        drawImage0(name, dateNow, timeNow, reservations);
    } else if (deviceType.compare("1") == 0) {
        drawImage1(name, dateNow, timeNow, reservations);
    } else if (deviceType.compare("2") == 0) {
        drawImage2(name, dateNow, timeNow, reservations);
    } else if (deviceType.compare("3") == 0) {
        drawImage3(name, dateNow, timeNow, reservations);
    }

    vector<unsigned char> compressed = compressImage();
    //write to a file
    ofstream(mac_address, ios::binary).write((const char*) image, x_res/8 * y_res);
    ofstream(mac_address + ".compressed", ios::binary).write((const char*) compressed.data(), compressed.size());

    //free memory
    delete canvas;
    return 0;
}
