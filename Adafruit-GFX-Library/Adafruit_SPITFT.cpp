/***************************************************
  This is our library for generic SPI TFT Displays with
  address windows and 16 bit color (e.g. ILI9341, HX8357D, ST7735...)

  Check out the links above for our tutorials and wiring diagrams
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional)
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/


#include "Adafruit_SPITFT.h"
#ifndef ARDUINO_STM32_FEATHER
#ifndef RASPI
#endif
#endif
#include <limits.h>



// Pass 8-bit (each) R,G,B, get back 16-bit packed color
uint16_t Adafruit_SPITFT::color565(uint8_t r, uint8_t g, uint8_t b) {
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | ((b & 0xF8) >> 3);
}

Adafruit_SPITFT::Adafruit_SPITFT(uint16_t w, uint16_t h,
				 int8_t cs, int8_t dc, int8_t mosi,
				 int8_t sclk, int8_t rst, int8_t miso) 
  : Adafruit_GFX(w, h) {
    _cs   = cs;
    _dc   = dc;
    _rst  = rst;
    _sclk = sclk;
    _mosi  = mosi;
    _miso = miso;
    _freq = 0;
#ifdef USE_FAST_PINIO
    csport    = portOutputRegister(digitalPinToPort(_cs));
    cspinmask = digitalPinToBitMask(_cs);
    dcport    = portOutputRegister(digitalPinToPort(_dc));
    dcpinmask = digitalPinToBitMask(_dc);
    clkport     = portOutputRegister(digitalPinToPort(_sclk));
    clkpinmask  = digitalPinToBitMask(_sclk);
    mosiport    = portOutputRegister(digitalPinToPort(_mosi));
    mosipinmask = digitalPinToBitMask(_mosi);
    if(miso >= 0){
        misoport    = portInputRegister(digitalPinToPort(_miso));
        misopinmask = digitalPinToBitMask(_miso);
    } else {
        misoport    = 0;
        misopinmask = 0;
    }
#endif
}

Adafruit_SPITFT::Adafruit_SPITFT(uint16_t w, uint16_t h,
				 int8_t cs, int8_t dc, int8_t rst) 
  : Adafruit_GFX(w, h) {
    _cs   = cs;
    _dc   = dc;
    _rst  = rst;
    _sclk  = -1;
    _mosi  = -1;
    _miso  = -1;
    _freq = 0;
#ifdef USE_FAST_PINIO
    csport    = portOutputRegister(digitalPinToPort(_cs));
    cspinmask = digitalPinToBitMask(_cs);
    dcport    = portOutputRegister(digitalPinToPort(_dc));
    dcpinmask = digitalPinToBitMask(_dc);
    clkport     = 0;
    clkpinmask  = 0;
    mosiport    = 0;
    mosipinmask = 0;
    misoport    = 0;
    misopinmask = 0;
#endif
}


/*
 * Transaction API
 * */


void Adafruit_SPITFT::writeCommand(uint8_t cmd){
}

void Adafruit_SPITFT::pushColor(uint16_t color) {
}


void Adafruit_SPITFT::writePixel(uint16_t color){
    //SPI_WRITE16(color);
}

void Adafruit_SPITFT::writePixels(uint16_t * colors, uint32_t len){
    //SPI_WRITE_PIXELS((uint8_t*)colors , len * 2);
}

void Adafruit_SPITFT::writeColor(uint16_t color, uint32_t len){
    if(_sclk >= 0){
        for (uint32_t t=0; t<len; t++){
            writePixel(color);
        }
        return;
    }
}

void Adafruit_SPITFT::writePixel(int16_t x, int16_t y, uint16_t color) {
    if((x < 0) ||(x >= _width) || (y < 0) || (y >= _height)) return;
    setAddrWindow(x,y,1,1);
    writePixel(color);
}

void Adafruit_SPITFT::writeFillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color){
    if((x >= _width) || (y >= _height)) return;
    int16_t x2 = x + w - 1, y2 = y + h - 1;
    if((x2 < 0) || (y2 < 0)) return;

    // Clip left/top
    if(x < 0) {
        x = 0;
        w = x2 + 1;
    }
    if(y < 0) {
        y = 0;
        h = y2 + 1;
    }

    // Clip right/bottom
    if(x2 >= _width)  w = _width  - x;
    if(y2 >= _height) h = _height - y;

    int32_t len = (int32_t)w * h;
    setAddrWindow(x, y, w, h);
    writeColor(color, len);
}

void Adafruit_SPITFT::writeFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color){
    writeFillRect(x, y, 1, h, color);
}

void Adafruit_SPITFT::writeFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color){
    writeFillRect(x, y, w, 1, color);
}

void Adafruit_SPITFT::drawPixel(int16_t x, int16_t y, uint16_t color){
    writePixel(x, y, color);
}

void Adafruit_SPITFT::drawFastVLine(int16_t x, int16_t y,
        int16_t h, uint16_t color) {
    writeFastVLine(x, y, h, color);
}

void Adafruit_SPITFT::drawFastHLine(int16_t x, int16_t y,
        int16_t w, uint16_t color) {
    writeFastHLine(x, y, w, color);
}

void Adafruit_SPITFT::fillRect(int16_t x, int16_t y, int16_t w, int16_t h,
        uint16_t color) {
    writeFillRect(x,y,w,h,color);
}

// Adapted from https://github.com/PaulStoffregen/ILI9341_t3
// by Marc MERLIN. See examples/pictureEmbed to use this.
// 5/6/2017: function name and arguments have changed for compatibility
// with current GFX library and to avoid naming problems in prior
// implementation.  Formerly drawBitmap() with arguments in different order.
void Adafruit_SPITFT::drawRGBBitmap(int16_t x, int16_t y,
  uint16_t *pcolors, int16_t w, int16_t h) {

    int16_t x2, y2; // Lower-right coord
    if(( x             >= _width ) ||      // Off-edge right
       ( y             >= _height) ||      // " top
       ((x2 = (x+w-1)) <  0      ) ||      // " left
       ((y2 = (y+h-1)) <  0)     ) return; // " bottom

    int16_t bx1=0, by1=0, // Clipped top-left within bitmap
            saveW=w;      // Save original bitmap width value
    if(x < 0) { // Clip left
        w  +=  x;
        bx1 = -x;
        x   =  0;
    }
    if(y < 0) { // Clip top
        h  +=  y;
        by1 = -y;
        y   =  0;
    }
    if(x2 >= _width ) w = _width  - x; // Clip right
    if(y2 >= _height) h = _height - y; // Clip bottom

    pcolors += by1 * saveW + bx1; // Offset bitmap ptr to clipped top-left
    setAddrWindow(x, y, w, h); // Clipped area
    while(h--) { // For each (clipped) scanline...
      writePixels(pcolors, w); // Push one (clipped) row
      pcolors += saveW; // Advance pointer by one full (unclipped) line
    }
}
