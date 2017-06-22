/******************************************************************
Nodemcu Weather Station

version: 1.0.0
author : lixplor
email  : me@lixplor.com

******************************************************************/

#include <Arduino.h>
#include "../utils/LogUtils.h"
#include "../device/DhtSensor.h"
#include "../device/PCD8544.h"
#include "../device/DustSensor.h"
using namespace std;

// 创建对象
LogUtils LogUtils;
DhtSensor DhtSensor;
PCD8544 PCD8544;
DustSensor DustSensor;


void setup()   {
    // 开启日志
    LogUtils.enableLog(true);

    // 开启串口
    Serial.begin(115200);

    // 初始化显示屏
    PCD8544.begin();
    PCD8544.setContrast(55);
    PCD8544.clearScreen();

    //初始化DHT11
    DhtSensor.begin();

    // 稳定
    delay(3000);
}


void loop() {

    float humid = DhtSensor.getHumid();
    float realTemp = DhtSensor.getRealTemp();
    float feelTemp = DhtSensor.getFeelTemp(realTemp, humid);

    LogUtils.d("READ HUMID:" + String(humid));
    LogUtils.d("READ REAL TEMP:" + String(realTemp));
    LogUtils.d("READ FEEL TEMP:" + String(feelTemp));

    double currentAqi = DustSensor.getCurrentAqi();
    LogUtils.d("current AQI:" + String(currentAqi));
    DustSensor.addList(currentAqi);
    double avgAqi = DustSensor.getAvgAQI();
    if (avgAqi == -1) {
        LogUtils.d("AQI data not enough!");
    } else {
        LogUtils.d("AQI=" + String(avgAqi));
    }
    

    PCD8544.clearScreen();

    PCD8544.text("06/11    23:30", 1, true);
    PCD8544.newLine();
    PCD8544.text("TEMP ", 1, false);
    PCD8544.text(String("       ") + (int)realTemp, 1, false);
    PCD8544.newLine();
    PCD8544.text("FEEL ", 1, false);
    PCD8544.text(String("       ") + (int)feelTemp, 1, false);
    PCD8544.newLine();
    PCD8544.text("HUMID", 1, false);
    PCD8544.text(String("       ") + (int)humid, 1, false);
    PCD8544.newLine();
    PCD8544.text("AQI  ", 1, false);
    int intAQI = (int)avgAqi;
    if (intAQI / 100 > 0) {
        // 三位数
        PCD8544.text(String("      " ) + intAQI, 1, false);
    } else if (intAQI / 10 > 0) {
        // 两位数
        PCD8544.text(String("       " ) + intAQI, 1, false);
    } else {
        // 一位数
        PCD8544.text(String("        " ) + intAQI, 1, false);
    }
    PCD8544.newLine();
    PCD8544.text("     HOME     ", 1, true);
    PCD8544.show();

    delay(2000);
}


