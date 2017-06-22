#ifndef DhtSensor_h
#define DhtSensor_h

#include <Arduino.h>
#include <SPI.h>
#include <DHT.h>

// 引脚定义
#define PIN_DHT 0        // DHT 11数据输出引脚
#define DHTTYPE DHT11    // DHT型号

class DhtSensor {
    // private:
        
    public:
        static DHT dht;
        static DHT getDHTInstance();
        static void begin();
        static float getHumid();
        static float getRealTemp();
        static float getFeelTemp(float realTemp, float humid);
        static float getDewPoint(float realTemp, float humid);
        static double Fahrenheit(double celsius);
        static double Kelvin(double celsius);
        static double dewPoint(double celsius, double humidity);
        static double dewPointFast(double celsius, double humidity);

};

#endif