#ifndef PCD8544_h
#define PCD8544_h

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

//ESP8266 with PCD8544 display

// 引脚定义
// If using an ESP8266, use this option. Comment out the other options.
// ESP8266 Hardware SPI (faster, but must use certain hardware pins):
// SCK is LCD serial clock (SCLK) - this is pin 14 on Huzzah ESP8266
// MOSI is LCD DIN - this is pin 13 on an Huzzah ESP8266
#define PIN_DC 12
#define PIN_CE 5
#define PIN_RST 4

// 配置
#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16



class PCD8544 {
    public:
        static Adafruit_PCD8544 display;
        static Adafruit_PCD8544 getDisplayInstance();
        // static unsigned char PROGMEM logo16_glcd_bmp[];
        void begin();
        void setContrast(int contrast);
        void clearScreen();
        void show();
        void text(String text, int fontSize, bool hasBackground);
        void textCenter(String text, int fontSize, bool hasBackground);
        void newLine();
        String genString(String title, int value);
        String genString(String title);
};

#endif