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
#include "../device/NodeMcu.h"
using namespace std;

// 创建对象
LogUtils LogUtils;
DhtSensor DhtSensor;
PCD8544 PCD8544;
DustSensor DustSensor;
NodeMcu nodeMcu;
ESP8266WebServer server(80);

void routerRoot() {
    String html = nodeMcu.HTML_FRONT + nodeMcu.HTML_END;

	server.send(200, "text/html", html);
}

void routerScan() {
    nodeMcu.disconnectWifi();
    int apNum = nodeMcu.scanAPNum();
    for (int i = 0; i < apNum; i++) {
        LogUtils.d(String(i + 1) + ": " + WiFi.SSID(i) + "(" + String(WiFi.RSSI()) + ")");
    }
    String html = nodeMcu.HTML_FRONT;
    for (int i = 0; i < apNum; i++) {
        html += nodeMcu.HTML_AP_NAME_FRONT + WiFi.SSID(i) + "'>" + WiFi.SSID(i) + nodeMcu.HTML_AP_NAME_END;
    }
    html += nodeMcu.HTML_END;
    server.send(200, "text/html", html);
}

void routerConnect() {
    int argNum = server.args();
    if (argNum > 0) {
        String wifiSsid = server.arg("wifiap");
        String wifiPwd = server.arg("password");
        LogUtils.d("ssid=" + wifiSsid + ", pwd=" + wifiPwd);
        char* pSsid = const_cast<char*>(wifiSsid.c_str());
        char* pBuff = const_cast<char*>(wifiPwd.c_str()); 
        LogUtils.d("ssid=" + String(pSsid) + ", pwd=" + pBuff);  
        nodeMcu.connectWifi(pSsid, pBuff);
        delay(1000);
        LogUtils.d("wifi connction: " + nodeMcu.getConnectionState());
        LogUtils.d("Connected ssid: " + nodeMcu.getWifiSSID());
        LogUtils.d("Connection rssi: " + String(nodeMcu.getWifiRSSI()));
        LogUtils.d("AP IP: ");
        Serial.println(nodeMcu.getAPIP());
        LogUtils.d("STA IP: ");
        Serial.println(nodeMcu.getSTAIP());
        server.send(200, "text/html", "连接成功");
    } else {
        server.send(200, "text/html", "wifi密码不正确");
    }
}

// 初始化显示屏
void initPCD() {
    PCD8544.begin();
    PCD8544.setContrast(55);
    PCD8544.clearScreen();
}

// 初始化NodeMcu
void initNodeMcu() {
    nodeMcu.setMode(WIFI_AP_STA);
    nodeMcu.startSoftAP("node-weather", "12345678");
    delay(5000);

    LogUtils.d("AP IP: ");
    Serial.println(nodeMcu.getAPIP());
    // LogUtils.d("STA IP: ");
    // Serial.println(nodeMcu.getSTAIP());

    server.on("/", routerRoot);
    server.on("/setting/wifi/scan", routerScan);
    server.on("/setting/wifi/connect", HTTPMethod::HTTP_POST, routerConnect);
    
    server.begin();

}

void setup()   {
    // 开启日志
    LogUtils.enableLog(true);

    // 开启串口
    Serial.begin(115200);

    // 初始化Wifi
    initNodeMcu();

    // 初始化显示屏
    initPCD();

    //初始化DHT11
    DhtSensor.begin();

    // 稳定
    delay(3000);
}


void loop() {

    server.handleClient();

    float humid = DhtSensor.getHumid();
    float realTemp = DhtSensor.getRealTemp();
    if (isnan(humid) || humid < 0 || humid > 100) {
        humid = 0;
    }
    if (isnan(realTemp) || realTemp < -100 || realTemp > 100) {
        realTemp = 0;
    }
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
    if (intAQI < 0) intAQI = 0;
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
    // 显示IP
    String ip = nodeMcu.getAPIP().toString();
    PCD8544.text(ip, 1, true);
    PCD8544.show();

    delay(2000);
}


