#ifndef LogUtils_h
#define LogUtils_h

#include "Arduino.h"

class LogUtils {
    public:
        static void enableLog(bool enable);
        static void d(String msg);
        // static void d(String msg, int dec);
    private:
        static bool canLog; 
        static String LOG_TITLE_VERBOSE;
        static String LOG_TITLE_DEBUG;
        static String LOG_TITLE_INFO;
        static String LOG_TITLE_WARN;
        static String LOG_TITLE_ERROR;
};

#endif