#ifndef IMAGE_h
#define IMAGE_h

#include <string>
#include <vector>
#include "layouts.h"

#define X_RES0 384
#define Y_RES0 640
#define X_RES1 400
#define Y_RES1 300
#define X_RES2 640
#define Y_RES2 384
#define X_RES3 640
#define Y_RES3 384
#define X_RES4 400
#define Y_RES4 300

struct reservation {
    public:
        std::string title;
        int startBlock;
        int endBlock;
};

void setPixel(int x, int y, unsigned char color);
void drawRect(int x, int y, int width, int height, unsigned char color);
void initializeImage();
void drawCharacter(int x, int y, char c);
void drawString(int x, int y, std::string str);
void drawFancyString(std::string str, int16_t x, int16_t y);
uint16_t getTextWidth(std::string str); 
bool drawCenteredString(std::string str, int16_t y);
void putQrCode(int x, int y, std::string str, int scale);
unsigned char reverseByte(unsigned char x);
void invert();
void rotate();
void mirror();
void flip();
std::string reservationBlockToTime(int block);
std::string militaryTimeToNormalPersonTime(std::string military);
std::string fancyDateFromYYYY_MM_DD(std::string YYYY_MM_DD);
std::vector<reservation> parseReservations(std::string* reservations);
void checkBattery(int xOffset, int yOffset, float voltage);
void setSleepTime(uint32_t increment); //increment is the target number of seconds between refreshes

#endif
