#include "PCD8544.h"

Adafruit_PCD8544 PCD8544::display = Adafruit_PCD8544(PIN_DC, PIN_CE, PIN_RST);

// 获取单例
Adafruit_PCD8544 PCD8544::getDisplayInstance() {
    Adafruit_PCD8544 *pDisplay = &PCD8544::display;
    if (pDisplay == NULL) {
        pDisplay = new Adafruit_PCD8544(PIN_DC, PIN_CE, PIN_RST);
    }
    return PCD8544::display;
}

// 设置 -------------------

// 启动
void PCD8544::begin() {
    display.begin();
}

// 设置对比度(推荐55)
void PCD8544::setContrast(int contrast) {
    display.setContrast(contrast);
}

// 清空显示
void PCD8544::clearScreen() {
    display.clearDisplay();
}

// 显示文字
void PCD8544::show() {
    display.display();
}

// 功能 -------------------

// 显示文字
void PCD8544::text(String text, int fontSize, bool hasBackground) {
    display.setTextSize(fontSize);
    if (hasBackground) {
        display.setTextColor(WHITE, BLACK);
    } else {
        display.setTextColor(BLACK);
    }
    display.print(text);
}

void PCD8544::newLine() {
    display.println();
}


// unsigned char PCD8544::PROGMEM logo16_glcd_bmp[] =
// { B00000000, B11000000,
//   B00000001, B11000000,
//   B00000001, B11000000,
//   B00000011, B11100000,
//   B11110011, B11100000,
//   B11111110, B11111000,
//   B01111110, B11111111,
//   B00110011, B10011111,
//   B00011111, B11111100,
//   B00001101, B01110000,
//   B00011011, B10100000,
//   B00111111, B11100000,
//   B00111111, B11110000,
//   B01111100, B11110000,
//   B01110000, B01110000,
//   B00000000, B00110000 };