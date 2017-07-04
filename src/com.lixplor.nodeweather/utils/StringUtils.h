#ifndef StringUtils_H
#define StringUtils_H

#include <Arduino.h>

class StringUtils {
    public:
        static const char* stringToConstChar(String str);
};

#endif