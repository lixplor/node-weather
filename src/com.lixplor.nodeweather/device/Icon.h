#ifndef Icon_H
#define Icon_H

#include <Arduino.h>

class Icon {
    public:
        static const unsigned char PROGMEM logo8_temp_bmp[];
        static const unsigned char PROGMEM logo8_feel_bmp[];
        static const unsigned char PROGMEM logo8_humid_bmp[];
        static const unsigned char PROGMEM logo8_aqi_bmp[];
        static const unsigned char PROGMEM logo8_wifi_bmp[];
        static const unsigned char PROGMEM logo8_location_bmp[];
        static const unsigned char PROGMEM logo8_c_bmp[];
        static const unsigned char PROGMEM logo8_percent_bmp[];
};

#endif