#ifndef DustSensor_h
#define DustSensor_h

#include <Arduino.h>
#include <GP2Y1010AU0F.h>
#include <vector>
using namespace std;

//引脚
#define PIN_LED 10       //灰尘传感器led供电
#define PIN_OUTPUT A0    //灰尘传感器模拟电压输出

const int AQI_INTERVAL = 1000;
const int AQI_SUM_DURATION = 1000 * 10 * 1;

class DustSensor {
    public:
        static GP2Y1010AU0F dustSensor;
        static vector<double> aqiQueue;
        static double getOutputV();
        static double calcDustDensity(double outputV);
        static double calcAQI(double dustDensity);
        static double getCurrentAqi();
        static void addList(double aqi);
        static double getAvgAQI();
        static int getAqiGrade(double aqi);
};

#endif