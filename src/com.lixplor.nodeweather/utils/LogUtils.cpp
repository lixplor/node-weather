/*
日志工具
*/

#include "LogUtils.h"
using namespace std;

// 日志开关
bool LogUtils::canLog = false;

// 日志标题
String LogUtils::LOG_TITLE_VERBOSE = "[VERBOSE] ";
String LogUtils::LOG_TITLE_DEBUG = "[DEBUG] ";
String LogUtils::LOG_TITLE_INFO = "[INFO] ";
String LogUtils::LOG_TITLE_WARN = "[WARN] ";
String LogUtils::LOG_TITLE_ERROR = "[ERROR] ";

// 是否开启日志
void LogUtils::enableLog(bool enable) {
    canLog = enable;
}

// 打印日志
void LogUtils::d(String msg) {
    if (canLog) {
        Serial.println(LOG_TITLE_DEBUG + msg);
    }
}

