#include "NodeMcu.h"

int NodeMcu::wifiStatus = WL_IDLE_STATUS;

// 功能 --------------------

// 设置WiFi模式: 热点:WIFI_AP, 客户端:WIFI_STA, 热点和客户端:WIFI_AP_STA, 关闭:WIFI_OFF
void NodeMcu::setMode(WiFiMode_t mode) {
    WiFi.mode(mode);
}

void NodeMcu::startSoftAP(const char* ssid, const char* password) {
    WiFi.softAP(ssid, password);
}

bool NodeMcu::connectWifi(char* ssid, char* password) {
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    return true;
    // for (int i = 0; i < 6; i++) {
    //     wifiStatus = WiFi.begin(ssid, password);
    //     Serial.println("wifi connecting: " + String(wifiStatus));
    //     delay(5000);
    //     if (wifiStatus == WL_CONNECTED) {
    //         return true;
    //     }
    // }
    // return false;
}

// 断开连接
void NodeMcu::disconnectWifi() {
    WiFi.disconnect();
}

// 扫描热点数量(扫描前要断开wifi)
int NodeMcu::scanAPNum() {
    return WiFi.scanNetworks();
}

// 获取信息 ------------------------

// 获取wifi连接状态
int NodeMcu::getConnectionState() {
    return wifiStatus;
}

// 获取作为AP的本机IP
IPAddress NodeMcu::getAPIP() {
    return WiFi.softAPIP();
}

// 获取作为STA的本机ip
IPAddress NodeMcu::getSTAIP() {
    return WiFi.localIP();
}

// 获取mac地址
byte* NodeMcu::getMac() {
    byte mac[6];
    WiFi.macAddress(mac);
    return mac;
}

// 获取本机Wifi ssid
String NodeMcu::getWifiSSID() {
    return WiFi.SSID();
}

// 获取wifi 信号强度
long NodeMcu::getWifiRSSI() {
    return WiFi.RSSI();
}

// 获取本机热点加密类型
// byte NodeMcu::getWifiEncType() {
//     return WiFi.encryptionType();
// }


// 服务端 ------------------

// 创建服务端
WiFiServer NodeMcu::createServer(uint16_t port) {
    WiFiServer server(port);
    server.begin();
    return server;
}

// 监听客户端, 阻塞
WiFiClient NodeMcu::listenToClient(WiFiServer server) {
    return server.available();
}

// 读取客户端请求
char NodeMcu::readClient(WiFiClient client) {
    return client.read();
}

// 响应客户端
void NodeMcu::response(WiFiClient client, String content) {
    client.println(content);
}

// 结束连接
void NodeMcu::closeConnection(WiFiClient client) {
    client.stop();
}


// 客户端 -------------------

bool NodeMcu::isClientConnected(WiFiClient client) {
    return client.connected();
}

// EEPROM --------------------

// EEPROM使用空间, 从0到4096
int NodeMcu::eepromSize;

// 初始化
void NodeMcu::initEeprom(int size) {
    eepromSize = size;
    EEPROM.begin(eepromSize);
}

// 读取EEPROM
byte NodeMcu::readEeprom(int address) {
    return EEPROM.read(address);
}

// 写入EEPROM, 地址从0开始. 一个地址写入一个byte
void NodeMcu::writeEeprom(int address, byte data) {
    EEPROM.write(address, data);
}

// 写入EEPROM, 字符串
void NodeMcu::writeEeprom(String data) {
    // todo
}

// 读取EEPROM, 字符串
String NodeMcu::readEeprom() {
    String result = "";
    for (int i = 0; i < eepromSize; i++) {
        byte b = readEeprom(i);
        result += String(b);
    }
    return result;
}

// 清空EEPROM
void NodeMcu::clearEeprom() {
    for (int i = 0; i < eepromSize; i++) {
        writeEeprom(i, 0);
    }
}

// HTTP --------------------

String NodeMcu::FINGERPRINT_HEWEATHER = "7D EB 04 77 9D 4C 80 13 62 E2 CE 39 CE A3 21 F9 3C";

// 创建客户端
// HTTPClient NodeMcu::createHttpClient() {
//     // HTTPClient http;
//     return http;
// }

// http request
// String NodeMcu::httpRequest(HTTPClient http, String method, String url, String data) {
//   Serial.println("HTTPS " + method + " to: " + url);
//   Serial.println("body=" + data);
//   http.begin(url, FINGERPRINT_HEWEATHER); //HTTPS
//   http.addHeader("X-HTTP-Method-Override", method, false, true);
//   int resultCode = http.POST(data);
//   String result = resultCode + "#" + http.getString();
//   Serial.println(result);
//   return result;
// }

String NodeMcu::httpGet(String url) {
  Serial.println("HTTPS GET to: " + url);
  http.begin(url, FINGERPRINT_HEWEATHER); //HTTPS
  int resultCode = http.GET();
  String result = http.getString();
  Serial.println(result);
  return result;
}

// String NodeMcu::httpPost(HTTPClient http, String url, String data) {
//   return httpRequest(http, "POST", url, data);
// }

// String NodeMcu::httpPut(HTTPClient http, String url, String data) {
//   return httpRequest(http, "PUT", url, data);
// }

// html --------------------

String NodeMcu::HTML_FRONT = "\
<!DOCTYPE html>\
<html>\
<head>\
    <meta charset='utf-8'/>\
    <title>设置页面</title>\
    <style>\
        body{\
            background-color: #121318;\
            color: #2190BE;\
        }\
        .frame {\
            background-color: #172E42;\
            border-radius: 0px;\
            border: 0px solid #1F95C5;\
            border-top: 20px solid #1F95C5;\
            padding: 8px;\
        }\
        .btn {\
            background-color: #193C51;\
            color: #2190BE;\
            border-radius: 0px;\
            border: 1px solid #1C6D90;\
        }\
        .btn:hover {\
            background-color: #1C6D90;\
        }\
        .input-text {\
            background-color: transparent;\
            border: 0px;\
            border-bottom: 1px solid #2190BE;\
            color: #2190BE;\
        }\
    </style>\
</head>\
<body>\
    <h1>Node-Weather</h1>\
    <p>已连接WiFi: wifi名</p>\
    <form method='get' action='/setting/wifi/scan'>\
        <input class='btn' type='submit' value='搜索无线网络'/>\
    </form>\
    <form class='frame' method='post' action='/setting/wifi/connect'>\
";

String NodeMcu::HTML_AP_NAME_FRONT = "        <div><input type='radio' name='wifiap' value='";

String NodeMcu::HTML_AP_NAME_END = "</input></div>";

String NodeMcu::HTML_END = "\
        <div>\
            <span>WiFi密码:</span>\
            <input class='input-text' type='password' name='password'/>\
        </div>\
        <input class='btn' type='submit' value='连接'/>\
    </form>\
</body>\
</html>\
";
