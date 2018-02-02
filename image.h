#ifndef IMAGE_h
#define IMAGE_h

#include <string>
#include <vector>

using namespace std;

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
        string title;
        int startBlock;
        int endBlock;
};

void setPixel(int x, int y, unsigned char color);
void drawRect(int x, int y, int width, int height, unsigned char color);
void initializeImage();
void drawCharacter(int x, int y, char c);
void drawString(int x, int y, string str);
void drawFancyString(string str, int16_t x, int16_t y);
bool drawCenteredString(string str, int16_t y);
unsigned char reverseByte(unsigned char x);
void invert();
void rotate();
void mirror();
void flip();
string reservationBlockToTime(int block);
string militaryTimeToNormalPersonTime(string military);
string fancyDateFromYYYY_MM_DD(string YYYY_MM_DD);
vector<reservation> parseReservations(string* reservations);
void checkBattery(int xOffset, int yOffset, float voltage);
void setSleepTime(uint32_t increment); //increment is the target number of seconds between refreshes
void drawImage0(string roomName, string date, string time, string* reservations, float voltage);
void drawImage1(string roomName, string date, string time, string* reservations, float voltage);
void drawImage2(string roomName, string date, string time, string* reservations, float voltage);
void drawImage3(string roomName, string date, string time, string* reservations, float voltage);
void drawImage4(string roomName, string date, string time, string* reservations, float voltage);

#endif
