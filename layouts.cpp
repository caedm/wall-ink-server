#include "layouts.h"
#include "image.h"
#include "fonts.h"
#include <sstream>
#define QR_CODE_BASE_URL "door-display.groups.et.byu.net/r.php?r="
#define BASE_SCHEDULING_URL "reserve.byu.edu"

extern GFXcanvas1* canvas;
extern uint16_t x_res;
extern uint16_t y_res;

using namespace std;

void drawImage0(string roomName, string date, string time, string* reservations, float voltage) { //portrait 7"
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
    drawCenteredString(fancyDateFromYYYY_MM_DD(date), 54);

    //Outer box
    drawRect(19,67,x_res - 19*2, y_res - 67*2, 1);
    drawRect(24,72,x_res - 24*2, y_res - 72*2, 0);

    drawCenteredString("Last updated " + militaryTimeToNormalPersonTime(time), y_res-80);
    drawCenteredString(BASE_SCHEDULING_URL, y_res-13);

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

    canvas->setFont(&FreeMono9pt7b);
    canvas->setTextColor(1);

    //For each time of day
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
        drawFancyString(time.str(), boxCoordinates[i][0] - 11, boxCoordinates[i][1]+19); 

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
    drawFancyString("Available",108,y_res-39);
    drawRect(211,y_res-59,51,28,1);
    drawFancyString("Reserved",264,y_res-39);

    checkBattery(x_res-100, y_res-100, voltage);
    //mirror();
}

void drawImage1(string roomName, string date, string time, string* reservations, float voltage) { //landscape 4", shows 2 appointments
    //set sleepTime
    setSleepTime(1800);

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
    if (currentBlock < 0)
        currentBlock = 0;
    if (currentBlock > 31)
        currentBlock = 31;

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

    checkBattery(x_res-64, y_res-44, voltage);
}

void drawImage2(string roomName, string date, string time, string* reservations, float voltage) { //7" landscape, shows 2 appointments plus blocks
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
    if (currentBlock < 0)
        currentBlock = 0;
    if (currentBlock > 31)
        currentBlock = 31;

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
        if (hourString.str().length() == 1)
            drawFancyString(hourString.str(), hour*40 - 245, 378);
        else
            drawFancyString(hourString.str(), hour*40 - 249, 378);
    }

    //draw blocks
    for (int block = 0; block < 32; block++) {
        drawRect(block*20, 337, 20, 22, 1);
        if (reservations[block].compare("Available") == 0) {
            //drawRect(block*20 + 1, 324, 18, 33, 0);
            //eliminate vertical lines
            drawRect(block*20, 339, 20, 18, 0);

            //put rounded corners on ends
            drawRect(0, 339, 2, 18, 1);
            drawRect(0, 337, 1, 2, 0);
            drawRect(1, 337, 1, 1, 0);
            drawRect(2, 339, 1, 1, 1);
            drawRect(0, 357, 1, 2, 0);
            drawRect(1, 358, 1, 1, 0);
            drawRect(2, 356, 1, 1, 1);

            drawRect(638, 339, 2, 18, 1);
            drawRect(639, 337, 1, 2, 0);
            drawRect(638, 337, 1, 1, 0);
            drawRect(637, 339, 1, 1, 1);
            drawRect(639, 357, 1, 2, 0);
            drawRect(638, 358, 1, 1, 0);
            drawRect(637, 356, 1, 1, 1);
        }
    }

    //round corners if edge case
    for (int block = 0; block < 32; block++) {
        if (reservations[block].compare("Available") == 0) {
            if (block > 0) {
                if (reservations[block-1].compare("Available") != 0) {
                    drawRect(block*20 - 4, 337, 5, 2, 0);
                    drawRect(block*20, 338, 1, 1, 1);
                    drawRect(block*20 - 4, 338, 1, 1, 1);
                    drawRect(block*20 - 4, 357, 5, 2, 0);
                    drawRect(block*20, 357, 1, 1, 1);
                    drawRect(block*20 - 4, 357, 1, 1, 1);
                    drawRect(block*20 - 2, 337, 1, 22, 0);
                    drawRect(block*20, 339, 1, 18, 1);
                    drawRect(block*20 + 1, 339, 1, 1, 1);
                    drawRect(block*20 + 1, 356, 1, 1, 1);
                }
            }
            if (block < 31) {
                if (reservations[block+1].compare("Available") != 0) {
                    drawRect(block*20 + 19, 337, 5, 2, 0);
                    drawRect(block*20 + 19, 338, 1, 1, 1);
                    drawRect(block*20 + 23, 338, 1, 1, 1);
                    drawRect(block*20 + 19, 357, 5, 2, 0);
                    drawRect(block*20 + 19, 357, 1, 1, 1);
                    drawRect(block*20 + 23, 357, 1, 1, 1);
                    drawRect(block*20 + 21, 337, 1, 22, 0);
                    drawRect(block*20 + 19, 339, 1, 18, 1);
                    drawRect(block*20 + 18, 339, 1, 1, 1);
                    drawRect(block*20 + 18, 356, 1, 1, 1);
                }
            }
        } else {
            if (block > 0) {
                if (reservations[block-1].compare(reservations[block]) != 0 && reservations[block-1].compare("Available") != 0) {
                    drawRect(block*20 - 2, 337, 5, 2, 0);
                    drawRect(block*20 + 2, 338, 1, 1, 1);
                    drawRect(block*20 - 2, 338, 1, 1, 1);
                    drawRect(block*20 - 2, 357, 5, 2, 0);
                    drawRect(block*20 + 2, 357, 1, 1, 1);
                    drawRect(block*20 - 2, 357, 1, 1, 1);
                    drawRect(block*20, 337, 1, 22, 0);
                }
            }
        }
    }

    //draw arrow drawRect((currentBlock-currentBlock%2)*20 - 1, 335, 2, 1, 1);
    drawRect((currentBlock-currentBlock%2)*20 - 2, 334, 4, 1, 1);
    drawRect((currentBlock-currentBlock%2)*20 - 3, 333, 6, 1, 1);
    drawRect((currentBlock-currentBlock%2)*20 - 4, 331, 8, 2, 1);

    //draw time above the arrow
    canvas->setFont(&FreeSansBold9pt7b);
    drawFancyString(militaryTimeToNormalPersonTime(currentStart), (currentBlock-currentBlock%2)*20 - 30, 325);

    checkBattery(x_res-100, y_res-100, voltage);
}

void drawImage3(string roomName, string date, string time, string* reservations, float voltage) { //7" landscape, shows 3 appointments plus blocks
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
    drawRect(0,95,x_res,2,1);

    //Get current block
    int currentBlock;
    currentBlock = (atoi(time.substr(0,2).c_str()) - 6) * 2;
    currentBlock += atoi(time.substr(3,2).c_str()) / 30;
    if (currentBlock < 0)
        currentBlock = 0;
    if (currentBlock > 31)
        currentBlock = 31;

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
		drawFancyString(prevEventTime, 9, 126);
		canvas->setFont(&FreeSans12pt7b);
        canvas->setTextWrap(true);
		drawFancyString(reservs.at(currentEventIndex-1).title, 8, 156);
        canvas->setTextWrap(false);
    }

    //Draw current event
    string currentEventTime = militaryTimeToNormalPersonTime(reservationBlockToTime(reservs.at(currentEventIndex).startBlock)) + " - " + militaryTimeToNormalPersonTime(reservationBlockToTime(reservs.at(currentEventIndex).endBlock));
    canvas->setFont(&FreeSansBold12pt7b);
    drawFancyString(currentEventTime, 8, 201);
    canvas->setFont(&FreeSans12pt7b);
    canvas->setTextWrap(true);
    drawFancyString(reservs.at(currentEventIndex).title, 8, 231);
    canvas->setTextWrap(false);

    //Draw next event
    if (reservs.size() > currentEventIndex+1) {
        string nextEventTime = militaryTimeToNormalPersonTime(reservationBlockToTime(reservs.at(currentEventIndex+1).startBlock)) + " - " + militaryTimeToNormalPersonTime(reservationBlockToTime(reservs.at(currentEventIndex+1).endBlock));
		canvas->setFont(&FreeSansBold12pt7b);
		drawFancyString(nextEventTime, 9, 276);
		canvas->setFont(&FreeSans12pt7b);
        canvas->setTextWrap(true);
		drawFancyString(reservs.at(currentEventIndex+1).title, 8, 306);
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
        if (hourString.str().length() == 1)
            drawFancyString(hourString.str(), hour*40 - 245, 378);
        else
            drawFancyString(hourString.str(), hour*40 - 249, 378);
    }

    //draw blocks
    for (int block = 0; block < 32; block++) {
        drawRect(block*20, 337, 20, 22, 1);
        if (reservations[block].compare("Available") == 0) {
            //drawRect(block*20 + 1, 324, 18, 33, 0);
            //eliminate vertical lines
            drawRect(block*20, 339, 20, 18, 0);

            //put rounded corners on ends
            drawRect(0, 339, 2, 18, 1);
            drawRect(0, 337, 1, 2, 0);
            drawRect(1, 337, 1, 1, 0);
            drawRect(2, 339, 1, 1, 1);
            drawRect(0, 357, 1, 2, 0);
            drawRect(1, 358, 1, 1, 0);
            drawRect(2, 356, 1, 1, 1);

            drawRect(638, 339, 2, 18, 1);
            drawRect(639, 337, 1, 2, 0);
            drawRect(638, 337, 1, 1, 0);
            drawRect(637, 339, 1, 1, 1);
            drawRect(639, 357, 1, 2, 0);
            drawRect(638, 358, 1, 1, 0);
            drawRect(637, 356, 1, 1, 1);
        }
    }

    //round corners if edge case
    for (int block = 0; block < 32; block++) {
        if (reservations[block].compare("Available") == 0) {
            if (block > 0) {
                if (reservations[block-1].compare("Available") != 0) {
                    drawRect(block*20 - 4, 337, 5, 2, 0);
                    drawRect(block*20, 338, 1, 1, 1);
                    drawRect(block*20 - 4, 338, 1, 1, 1);
                    drawRect(block*20 - 4, 357, 5, 2, 0);
                    drawRect(block*20, 357, 1, 1, 1);
                    drawRect(block*20 - 4, 357, 1, 1, 1);
                    drawRect(block*20 - 2, 337, 1, 22, 0);
                    drawRect(block*20, 339, 1, 18, 1);
                    drawRect(block*20 + 1, 339, 1, 1, 1);
                    drawRect(block*20 + 1, 356, 1, 1, 1);
                }
            }
            if (block < 31) {
                if (reservations[block+1].compare("Available") != 0) {
                    drawRect(block*20 + 19, 337, 5, 2, 0);
                    drawRect(block*20 + 19, 338, 1, 1, 1);
                    drawRect(block*20 + 23, 338, 1, 1, 1);
                    drawRect(block*20 + 19, 357, 5, 2, 0);
                    drawRect(block*20 + 19, 357, 1, 1, 1);
                    drawRect(block*20 + 23, 357, 1, 1, 1);
                    drawRect(block*20 + 21, 337, 1, 22, 0);
                    drawRect(block*20 + 19, 339, 1, 18, 1);
                    drawRect(block*20 + 18, 339, 1, 1, 1);
                    drawRect(block*20 + 18, 356, 1, 1, 1);
                }
            }
        } else {
            if (block > 0) {
                if (reservations[block-1].compare(reservations[block]) != 0 && reservations[block-1].compare("Available") != 0) {
                    drawRect(block*20 - 2, 337, 5, 2, 0);
                    drawRect(block*20 + 2, 338, 1, 1, 1);
                    drawRect(block*20 - 2, 338, 1, 1, 1);
                    drawRect(block*20 - 2, 357, 5, 2, 0);
                    drawRect(block*20 + 2, 357, 1, 1, 1);
                    drawRect(block*20 - 2, 357, 1, 1, 1);
                    drawRect(block*20, 337, 1, 22, 0);
                }
            }
        }
    }
    
    //draw arrow
    drawRect((currentBlock-currentBlock%2)*20, 335, 2, 1, 1);
    drawRect((currentBlock-currentBlock%2)*20 - 1, 334, 4, 1, 1);
    drawRect((currentBlock-currentBlock%2)*20 - 2, 333, 6, 1, 1);
    drawRect((currentBlock-currentBlock%2)*20 - 3, 331, 8, 2, 1);

    //draw time above the arrow
    //canvas->setFont(&FreeSansBold9pt7b);
    //drawFancyString(militaryTimeToNormalPersonTime(reservationBlockToTime(currentBlock-currentBlock%2)), (currentBlock-currentBlock%2)*20 - 30, 326);

    checkBattery(x_res-100, y_res-100, voltage);
}

void drawImage4(string roomName, string date, string time, string* reservations, float voltage, string resourceID) { //landscape 4", shows 2 appointments
    //set sleepTime
    setSleepTime(1800);

    //Draw room name and date
    canvas->setFont(&FreeSansBold12pt7b);
    canvas->setTextColor(1);
    canvas->setTextWrap(false);
    drawCenteredString(roomName, 26);
    drawCenteredString(fancyDateFromYYYY_MM_DD(date), 53);

    //Draw dividing line
    drawRect(0,63,x_res,2,1);

    //Get current block
    int currentBlock;
    currentBlock = (atoi(time.substr(0,2).c_str()) - 6) * 2;
    currentBlock += atoi(time.substr(3,2).c_str()) / 30;
    if (currentBlock < 0)
        currentBlock = 0;
    if (currentBlock > 31)
        currentBlock = 31;

    //Get current event
    string currentTitle = reservations[currentBlock];
    string currentStart = reservationBlockToTime(currentBlock);
    int blockNextStart = currentBlock;
    while (currentTitle.compare(reservations[++blockNextStart]) == 0 && blockNextStart < 32) {}
    string currentEnd = reservationBlockToTime(blockNextStart);
    
    //Draw current event
    string currentEventTime = militaryTimeToNormalPersonTime(currentStart) + " - " + militaryTimeToNormalPersonTime(currentEnd);
    canvas->setFont(&FreeSansBold18pt7b);
    drawFancyString(currentEventTime, 8, 101);
    canvas->setFont(&FreeSans18pt7b);
    canvas->setTextWrap(false);
    drawFancyString(currentTitle, 8, 141); canvas->setTextWrap(false); 
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
		drawFancyString(nextEventTime, 9, 191);
		canvas->setFont(&FreeSans12pt7b);
        canvas->setTextWrap(false);
		drawFancyString(nextTitle, 8, 221);
        canvas->setTextWrap(false);
    }

    checkBattery(x_res-64, y_res-44, voltage);
    
    //draw times
    canvas->setFont(&FreeSansBold9pt7b);
    for (int hour = 7; hour < 22; hour++) {
        if (hour % 2 == 0) {
            stringstream hourString;
            if (hour < 13)
                hourString << hour;
            else
                hourString << hour-12;
            if (hourString.str().length() == 1)
                drawFancyString(hourString.str(), hour*24 - 143, 294);
            else
                drawFancyString(hourString.str(), hour*24 - 147, 294);
        }
    }

    //draw blocks
    for (int block = 0; block < 32; block++) {
        drawRect(block*12+8, 259, 12, 16, 1);
        if (reservations[block].compare("Available") == 0) {
            
            //eliminate vertical lines
            drawRect(block*12+8, 261, 12, 12, 0);

            //put rounded corners on ends
            drawRect(8, 261, 2, 12, 1);
            drawRect(8, 259, 1, 2, 0);
            drawRect(9, 259, 1, 1, 0);
            drawRect(10, 261, 1, 1, 1);
            drawRect(8, 273, 1, 2, 0);
            drawRect(9, 274, 1, 1, 0);
            drawRect(10, 272, 1, 1, 1);

            drawRect(390, 261, 2, 12, 1);
            drawRect(391, 259, 1, 2, 0);
            drawRect(390, 259, 1, 1, 0);
            drawRect(389, 261, 1, 1, 1);
            drawRect(391, 273, 1, 2, 0);
            drawRect(390, 274, 1, 1, 0);
            drawRect(389, 272, 1, 1, 1);
        }
    }

    //round corners if edge case
    for (int block = 0; block < 32; block++) {
        if (reservations[block].compare("Available") == 0) {
            if (block > 0) {
                if (reservations[block-1].compare("Available") != 0) {
                    drawRect(block*12 + 5, 259, 5, 2, 0);
                    drawRect(block*12 + 9, 260, 1, 1, 1);
                    drawRect(block*12 + 5, 260, 1, 1, 1);
                    drawRect(block*12 + 5, 273, 5, 2, 0);
                    drawRect(block*12 + 9, 273, 1, 1, 1);
                    drawRect(block*12 + 5, 273, 1, 1, 1);
                    drawRect(block*12 + 7, 259, 1, 16, 0);
                    drawRect(block*12 + 8, 261, 2, 12, 1);
                    drawRect(block*12 + 10, 261, 1, 1, 1);
                    drawRect(block*12 + 10, 272, 1, 1, 1);
                }
            }
            if (block < 31) {
                if (reservations[block+1].compare("Available") != 0) {
                    drawRect(block*12 + 18, 259, 5, 2, 0);
                    drawRect(block*12 + 18, 260, 1, 1, 1);
                    drawRect(block*12 + 22, 260, 1, 1, 1);
                    drawRect(block*12 + 18, 273, 5, 2, 0);
                    drawRect(block*12 + 18, 273, 1, 1, 1);
                    drawRect(block*12 + 22, 273, 1, 1, 1);
                    drawRect(block*12 + 20, 259, 1, 16, 0);
                    drawRect(block*12 + 18, 261, 2, 12, 1);
                    drawRect(block*12 + 17, 261, 1, 1, 1);
                    drawRect(block*12 + 17, 272, 1, 1, 1);
                }
            }
        } else {
            if (block > 0) {
                if (reservations[block-1].compare(reservations[block]) != 0 && reservations[block-1].compare("Available") != 0) {
                    drawRect(block*12 + 6, 259, 5, 2, 0);
                    drawRect(block*12 + 10, 260, 1, 1, 1);
                    drawRect(block*12 + 6, 260, 1, 1, 1);
                    drawRect(block*12 + 6, 273, 5, 2, 0);
                    drawRect(block*12 + 10, 273, 1, 1, 1);
                    drawRect(block*12 + 6, 273, 1, 1, 1);
                    drawRect(block*12 + 8, 259, 1, 16, 0);
                }
            }
        }
    }
    
    //draw arrow
    drawRect((currentBlock-currentBlock%2)*12 + 8, 257, 2, 1, 1);
    drawRect((currentBlock-currentBlock%2)*12 + 7, 256, 4, 1, 1);
    drawRect((currentBlock-currentBlock%2)*12 + 6, 255, 6, 1, 1);
    drawRect((currentBlock-currentBlock%2)*12 + 5, 253, 8, 2, 1);
}

//layout 5 is for static 7" images

void drawImage6(string roomName, string date, string time, string* reservations, float voltage, string resourceID) { //landscape 4", shows 2 appointments and has QR code
    //set sleepTime
    setSleepTime(1800);

    //Draw room name and date
    canvas->setFont(&FreeSansBold12pt7b);
    canvas->setTextColor(1);
    canvas->setTextWrap(false);
    drawFancyString(roomName,9,30);
    canvas->setFont(&FreeSansBold12pt7b);
    drawFancyString(fancyDateFromYYYY_MM_DD(date),9,61);

    //Draw rectangle for title emphasis
    drawRect(0,0,6,76,3);

    //Draw dividing line
    drawRect(6,74,x_res-6,2,1);

    //Get current block
    int currentBlock;
    currentBlock = (atoi(time.substr(0,2).c_str()) - 6) * 2;
    currentBlock += atoi(time.substr(3,2).c_str()) / 30;
    if (currentBlock < 0)
        currentBlock = 0;
    if (currentBlock > 31)
        currentBlock = 31;

    //Get current event
    string currentTitle = reservations[currentBlock];
    string currentStart = reservationBlockToTime(currentBlock);
    int blockNextStart = currentBlock;
    while (currentTitle.compare(reservations[++blockNextStart]) == 0 && blockNextStart < 32) {}
    string currentEnd = reservationBlockToTime(blockNextStart);
    
    //Draw current event
    string currentEventTime = militaryTimeToNormalPersonTime(currentStart) + " - " + militaryTimeToNormalPersonTime(currentEnd);
    canvas->setFont(&FreeSansBold18pt7b);
    drawFancyString(currentEventTime, 8, 114);
    canvas->setFont(&FreeSans18pt7b);
    canvas->setTextWrap(false);
    drawFancyString(currentTitle, 8, 154); canvas->setTextWrap(false); 
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
		drawFancyString(nextEventTime, 9, 199);
		canvas->setFont(&FreeSans12pt7b);
        canvas->setTextWrap(false);
		drawFancyString(nextTitle, 8, 229);
        canvas->setTextWrap(false);
    }

    
    //draw times
    canvas->setFont(&FreeSansBold9pt7b);
    for (int hour = 7; hour < 22; hour++) {
        if (hour % 2 == 0) {
            stringstream hourString;
            if (hour < 13)
                hourString << hour;
            else
                hourString << hour-12;
            if (hourString.str().length() == 1)
                drawFancyString(hourString.str(), hour*24 - 143, 299);
            else
                drawFancyString(hourString.str(), hour*24 - 147, 299);
        }
    }

    //draw blocks
    for (int block = 0; block < 32; block++) {
        drawRect(block*12+8, 259, 12, 16, 1);
        if (reservations[block].compare("Available") == 0) {
            
            //eliminate vertical lines
            drawRect(block*12+8, 261, 12, 12, 0);

            //put rounded corners on ends
            drawRect(8, 261, 2, 12, 1);
            drawRect(8, 259, 1, 2, 0);
            drawRect(9, 259, 1, 1, 0);
            drawRect(10, 261, 1, 1, 1);
            drawRect(8, 273, 1, 2, 0);
            drawRect(9, 274, 1, 1, 0);
            drawRect(10, 272, 1, 1, 1);

            drawRect(390, 261, 2, 12, 1);
            drawRect(391, 259, 1, 2, 0);
            drawRect(390, 259, 1, 1, 0);
            drawRect(389, 261, 1, 1, 1);
            drawRect(391, 273, 1, 2, 0);
            drawRect(390, 274, 1, 1, 0);
            drawRect(389, 272, 1, 1, 1);
        }
    }

    //round corners if edge case
    for (int block = 0; block < 32; block++) {
        if (reservations[block].compare("Available") == 0) {
            if (block > 0) {
                if (reservations[block-1].compare("Available") != 0) {
                    drawRect(block*12 + 5, 259, 5, 2, 0);
                    drawRect(block*12 + 9, 260, 1, 1, 1);
                    drawRect(block*12 + 5, 260, 1, 1, 1);
                    drawRect(block*12 + 5, 273, 5, 2, 0);
                    drawRect(block*12 + 9, 273, 1, 1, 1);
                    drawRect(block*12 + 5, 273, 1, 1, 1);
                    drawRect(block*12 + 7, 259, 1, 16, 0);
                    drawRect(block*12 + 8, 261, 2, 12, 1);
                    drawRect(block*12 + 10, 261, 1, 1, 1);
                    drawRect(block*12 + 10, 272, 1, 1, 1);
                }
            }
            if (block < 31) {
                if (reservations[block+1].compare("Available") != 0) {
                    drawRect(block*12 + 18, 259, 5, 2, 0);
                    drawRect(block*12 + 18, 260, 1, 1, 1);
                    drawRect(block*12 + 22, 260, 1, 1, 1);
                    drawRect(block*12 + 18, 273, 5, 2, 0);
                    drawRect(block*12 + 18, 273, 1, 1, 1);
                    drawRect(block*12 + 22, 273, 1, 1, 1);
                    drawRect(block*12 + 20, 259, 1, 16, 0);
                    drawRect(block*12 + 18, 261, 2, 12, 1);
                    drawRect(block*12 + 17, 261, 1, 1, 1);
                    drawRect(block*12 + 17, 272, 1, 1, 1);
                }
            }
        } else {
            if (block > 0) {
                if (reservations[block-1].compare(reservations[block]) != 0 && reservations[block-1].compare("Available") != 0) {
                    drawRect(block*12 + 6, 259, 5, 2, 0);
                    drawRect(block*12 + 10, 260, 1, 1, 1);
                    drawRect(block*12 + 6, 260, 1, 1, 1);
                    drawRect(block*12 + 6, 273, 5, 2, 0);
                    drawRect(block*12 + 10, 273, 1, 1, 1);
                    drawRect(block*12 + 6, 273, 1, 1, 1);
                    drawRect(block*12 + 8, 259, 1, 16, 0);
                }
            }
        }
    }
    
    //draw arrow
    drawRect((currentBlock-currentBlock%2)*12 + 8, 257, 2, 1, 1);
    drawRect((currentBlock-currentBlock%2)*12 + 7, 256, 4, 1, 1);
    drawRect((currentBlock-currentBlock%2)*12 + 6, 255, 6, 1, 1);
    drawRect((currentBlock-currentBlock%2)*12 + 5, 253, 8, 2, 1);

    putQrCode(333,10,QR_CODE_BASE_URL + resourceID, 2);

    checkBattery(x_res-64, y_res-44, voltage);
}

void drawImage7(string roomName, string date, string time, string* reservations, float voltage, string resourceID) { //7" landscape, shows 2 appointments plus blocks & a QR code
    //set sleepTime
    setSleepTime(1800);

    canvas->setFont(&FreeSansBold18pt7b);
    canvas->setTextColor(1);
    canvas->setTextWrap(false);

    //parse reservations
    vector<reservation> reservs = parseReservations(reservations);

    //draw room name
    drawFancyString(roomName, 15, 45);

    //draw date
    canvas->setFont(&FreeSansBold12pt7b);
    drawFancyString(fancyDateFromYYYY_MM_DD(date), 15, 80);

    //draw rectangle to emphasize title block
    drawRect(0,0,9,98,3);

    //draw line under date
    canvas->setFont(&FreeSansBold9pt7b);
    drawRect(9,95,x_res-9,3,1);
    drawRect(624-getTextWidth(BASE_SCHEDULING_URL)-12,95,getTextWidth(BASE_SCHEDULING_URL)+12,3,0);

    //draw base scheduling url
    drawFancyString(BASE_SCHEDULING_URL,617 - getTextWidth(BASE_SCHEDULING_URL),100);

    putQrCode(560,20,QR_CODE_BASE_URL + resourceID, 2);

    //Get current block
    int currentBlock;
    currentBlock = (atoi(time.substr(0,2).c_str()) - 6) * 2;
    currentBlock += atoi(time.substr(3,2).c_str()) / 30;
    if (currentBlock < 0)
        currentBlock = 0;
    if (currentBlock > 31)
        currentBlock = 31;

    //Get current event
    int currentEventIndex;
    for (int i = 0; i < reservs.size(); i++) {
        if (currentBlock >= reservs.at(i).startBlock && currentBlock <= reservs.at(i).endBlock)
            currentEventIndex = i;
    }
    
    //Draw current event
    string currentEventTime = militaryTimeToNormalPersonTime(reservationBlockToTime(currentBlock)) + " - " + militaryTimeToNormalPersonTime(reservationBlockToTime(reservs.at(currentEventIndex).endBlock));
    canvas->setFont(&FreeSansBold18pt7b);
    drawFancyString(currentEventTime, 15, 142);
    canvas->setFont(&FreeSans18pt7b);
    canvas->setTextWrap(true);
    drawFancyString(reservs.at(currentEventIndex).title, 15, 178);
    canvas->setTextWrap(false);

    //Draw next event
    if (reservs.size() > currentEventIndex+1) {
        string nextEventTime = militaryTimeToNormalPersonTime(reservationBlockToTime(reservs.at(currentEventIndex+1).startBlock)) + " - " + militaryTimeToNormalPersonTime(reservationBlockToTime(reservs.at(currentEventIndex+1).endBlock));
		canvas->setFont(&FreeSansBold12pt7b);
		drawFancyString(nextEventTime, 15, 221);
		canvas->setFont(&FreeSans12pt7b);
        canvas->setTextWrap(true);
		drawFancyString(reservs.at(currentEventIndex+1).title, 15, 251);
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
        if (hourString.str().length() == 1)
            drawFancyString(hourString.str(), hour*40 - 245, 378);
        else
            drawFancyString(hourString.str(), hour*40 - 249, 378);
    }

    //draw blocks
    for (int block = 0; block < 32; block++) {
        drawRect(block*20, 337, 20, 22, 1);
        if (reservations[block].compare("Available") == 0) {
            //drawRect(block*20 + 1, 324, 18, 33, 0);
            //eliminate vertical lines
            drawRect(block*20, 339, 20, 18, 0);

            //put rounded corners on ends
            drawRect(0, 339, 2, 18, 1);
            drawRect(0, 337, 1, 2, 0);
            drawRect(1, 337, 1, 1, 0);
            drawRect(2, 339, 1, 1, 1);
            drawRect(0, 357, 1, 2, 0);
            drawRect(1, 358, 1, 1, 0);
            drawRect(2, 356, 1, 1, 1);

            drawRect(638, 339, 2, 18, 1);
            drawRect(639, 337, 1, 2, 0);
            drawRect(638, 337, 1, 1, 0);
            drawRect(637, 339, 1, 1, 1);
            drawRect(639, 357, 1, 2, 0);
            drawRect(638, 358, 1, 1, 0);
            drawRect(637, 356, 1, 1, 1);
        }
    }

    //round corners if edge case
    for (int block = 0; block < 32; block++) {
        if (reservations[block].compare("Available") == 0) {
            if (block > 0) {
                if (reservations[block-1].compare("Available") != 0) {
                    drawRect(block*20 - 4, 337, 5, 2, 0);
                    drawRect(block*20, 338, 1, 1, 1);
                    drawRect(block*20 - 4, 338, 1, 1, 1);
                    drawRect(block*20 - 4, 357, 5, 2, 0);
                    drawRect(block*20, 357, 1, 1, 1);
                    drawRect(block*20 - 4, 357, 1, 1, 1);
                    drawRect(block*20 - 2, 337, 1, 22, 0);
                    drawRect(block*20, 339, 1, 18, 1);
                    drawRect(block*20 + 1, 339, 1, 1, 1);
                    drawRect(block*20 + 1, 356, 1, 1, 1);
                }
            }
            if (block < 31) {
                if (reservations[block+1].compare("Available") != 0) {
                    drawRect(block*20 + 19, 337, 5, 2, 0);
                    drawRect(block*20 + 19, 338, 1, 1, 1);
                    drawRect(block*20 + 23, 338, 1, 1, 1);
                    drawRect(block*20 + 19, 357, 5, 2, 0);
                    drawRect(block*20 + 19, 357, 1, 1, 1);
                    drawRect(block*20 + 23, 357, 1, 1, 1);
                    drawRect(block*20 + 21, 337, 1, 22, 0);
                    drawRect(block*20 + 19, 339, 1, 18, 1);
                    drawRect(block*20 + 18, 339, 1, 1, 1);
                    drawRect(block*20 + 18, 356, 1, 1, 1);
                }
            }
        } else {
            if (block > 0) {
                if (reservations[block-1].compare(reservations[block]) != 0 && reservations[block-1].compare("Available") != 0) {
                    drawRect(block*20 - 2, 337, 5, 2, 0);
                    drawRect(block*20 + 2, 338, 1, 1, 1);
                    drawRect(block*20 - 2, 338, 1, 1, 1);
                    drawRect(block*20 - 2, 357, 5, 2, 0);
                    drawRect(block*20 + 2, 357, 1, 1, 1);
                    drawRect(block*20 - 2, 357, 1, 1, 1);
                    drawRect(block*20, 337, 1, 22, 0);
                }
            }
        }
    }
    
    //draw arrow
    drawRect((currentBlock-currentBlock%2)*20, 335, 2, 1, 1);
    drawRect((currentBlock-currentBlock%2)*20 - 1, 334, 4, 1, 1);
    drawRect((currentBlock-currentBlock%2)*20 - 2, 333, 6, 1, 1);
    drawRect((currentBlock-currentBlock%2)*20 - 3, 331, 8, 2, 1);

    //draw time above the arrow
    //canvas->setFont(&FreeSansBold9pt7b);
    //drawFancyString(militaryTimeToNormalPersonTime(reservationBlockToTime(currentBlock-currentBlock%2)), (currentBlock-currentBlock%2)*20 - 30, 326);

    checkBattery(x_res-100, y_res-100, voltage);
}

//layout 8 is for static 4" images

void drawImage9(string roomName, string date, string time, string* reservations, float voltage, string resourceID) { //landscape 4", shows 2 appointments and has QR code. More battery efficient than layout 6.
    //set sleepTime
    setSleepTime(1800);

    //parse reservations
    vector<reservation> reservs = parseReservations(reservations);

    //Draw room name and date
    canvas->setFont(&FreeSansBold12pt7b);
    canvas->setTextColor(1);
    canvas->setTextWrap(false);
    drawFancyString(roomName,9,30);
    canvas->setFont(&FreeSansBold12pt7b);
    drawFancyString(fancyDateFromYYYY_MM_DD(date),9,61);

    //Draw rectangle for title emphasis
    drawRect(0,0,6,76,3);

    //Draw dividing line
    drawRect(6,74,x_res-6,2,1);

    //Get current block
    int currentBlock;
    currentBlock = (atoi(time.substr(0,2).c_str()) - 6) * 2;
    currentBlock += atoi(time.substr(3,2).c_str()) / 30;
    if (currentBlock < 0)
        currentBlock = 0;
    if (currentBlock > 31)
        currentBlock = 31;
    
    //Get current event
    int currentEventIndex;
    for (int i = 0; i < reservs.size(); i++) {
        if (currentBlock >= reservs.at(i).startBlock && currentBlock <= reservs.at(i).endBlock)
            currentEventIndex = i;
    }

    //Get current event
    string currentTitle = reservations[currentBlock];
    string currentStart = reservationBlockToTime(currentBlock);
    int blockNextStart = currentBlock;
    while (currentTitle.compare(reservations[++blockNextStart]) == 0 && blockNextStart < 32) {}
    string currentEnd = reservationBlockToTime(blockNextStart);
    
    //Draw current event
    string currentEventTime = militaryTimeToNormalPersonTime(reservationBlockToTime(reservs.at(currentEventIndex).startBlock)) + " - " + militaryTimeToNormalPersonTime(currentEnd);
    canvas->setFont(&FreeSansBold18pt7b);
    drawFancyString(currentEventTime, 8, 114);
    canvas->setFont(&FreeSans18pt7b);
    canvas->setTextWrap(false);
    drawFancyString(currentTitle, 8, 154); canvas->setTextWrap(false); 
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
		drawFancyString(nextEventTime, 9, 199);
		canvas->setFont(&FreeSans12pt7b);
        canvas->setTextWrap(false);
		drawFancyString(nextTitle, 8, 229);
        canvas->setTextWrap(false);
    }

    
    //draw times
    canvas->setFont(&FreeSansBold9pt7b);
    for (int hour = 7; hour < 22; hour++) {
        if (hour % 2 == 0) {
            stringstream hourString;
            if (hour < 13)
                hourString << hour;
            else
                hourString << hour-12;
            if (hourString.str().length() == 1)
                drawFancyString(hourString.str(), hour*24 - 143, 299);
            else
                drawFancyString(hourString.str(), hour*24 - 147, 299);
        }
    }

    //draw blocks
    for (int block = 0; block < 32; block++) {
        drawRect(block*12+8, 259, 12, 16, 1);
        if (reservations[block].compare("Available") == 0) {
            
            //eliminate vertical lines
            drawRect(block*12+8, 261, 12, 12, 0);

            //put rounded corners on ends
            drawRect(8, 261, 2, 12, 1);
            drawRect(8, 259, 1, 2, 0);
            drawRect(9, 259, 1, 1, 0);
            drawRect(10, 261, 1, 1, 1);
            drawRect(8, 273, 1, 2, 0);
            drawRect(9, 274, 1, 1, 0);
            drawRect(10, 272, 1, 1, 1);

            drawRect(390, 261, 2, 12, 1);
            drawRect(391, 259, 1, 2, 0);
            drawRect(390, 259, 1, 1, 0);
            drawRect(389, 261, 1, 1, 1);
            drawRect(391, 273, 1, 2, 0);
            drawRect(390, 274, 1, 1, 0);
            drawRect(389, 272, 1, 1, 1);
        }
    }

    //round corners if edge case
    for (int block = 0; block < 32; block++) {
        if (reservations[block].compare("Available") == 0) {
            if (block > 0) {
                if (reservations[block-1].compare("Available") != 0) {
                    drawRect(block*12 + 5, 259, 5, 2, 0);
                    drawRect(block*12 + 9, 260, 1, 1, 1);
                    drawRect(block*12 + 5, 260, 1, 1, 1);
                    drawRect(block*12 + 5, 273, 5, 2, 0);
                    drawRect(block*12 + 9, 273, 1, 1, 1);
                    drawRect(block*12 + 5, 273, 1, 1, 1);
                    drawRect(block*12 + 7, 259, 1, 16, 0);
                    drawRect(block*12 + 8, 261, 2, 12, 1);
                    drawRect(block*12 + 10, 261, 1, 1, 1);
                    drawRect(block*12 + 10, 272, 1, 1, 1);
                }
            }
            if (block < 31) {
                if (reservations[block+1].compare("Available") != 0) {
                    drawRect(block*12 + 18, 259, 5, 2, 0);
                    drawRect(block*12 + 18, 260, 1, 1, 1);
                    drawRect(block*12 + 22, 260, 1, 1, 1);
                    drawRect(block*12 + 18, 273, 5, 2, 0);
                    drawRect(block*12 + 18, 273, 1, 1, 1);
                    drawRect(block*12 + 22, 273, 1, 1, 1);
                    drawRect(block*12 + 20, 259, 1, 16, 0);
                    drawRect(block*12 + 18, 261, 2, 12, 1);
                    drawRect(block*12 + 17, 261, 1, 1, 1);
                    drawRect(block*12 + 17, 272, 1, 1, 1);
                }
            }
        } else {
            if (block > 0) {
                if (reservations[block-1].compare(reservations[block]) != 0 && reservations[block-1].compare("Available") != 0) {
                    drawRect(block*12 + 6, 259, 5, 2, 0);
                    drawRect(block*12 + 10, 260, 1, 1, 1);
                    drawRect(block*12 + 6, 260, 1, 1, 1);
                    drawRect(block*12 + 6, 273, 5, 2, 0);
                    drawRect(block*12 + 10, 273, 1, 1, 1);
                    drawRect(block*12 + 6, 273, 1, 1, 1);
                    drawRect(block*12 + 8, 259, 1, 16, 0);
                }
            }
        }
    }
    
    //draw arrow
    //drawRect((currentBlock-currentBlock%2)*12 + 8, 257, 2, 1, 1);
    //drawRect((currentBlock-currentBlock%2)*12 + 7, 256, 4, 1, 1);
    //drawRect((currentBlock-currentBlock%2)*12 + 6, 255, 6, 1, 1);
    //drawRect((currentBlock-currentBlock%2)*12 + 5, 253, 8, 2, 1);

    putQrCode(333,10,QR_CODE_BASE_URL + resourceID, 2);

    checkBattery(x_res-64, y_res-44, voltage);
}
