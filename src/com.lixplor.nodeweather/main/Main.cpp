/******************************************************************
Nodemcu Weather Station

version: 1.0.0
author : lixplor
email  : me@lixplor.com

******************************************************************/

#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>

#include "../utils/LogUtils.h"
#include "../utils/StringUtils.h"
#include "../device/DhtSensor.h"
#include "../device/PCD8544.h"
#include "../device/DustSensor.h"
#include "../device/NodeMcu.h"
using namespace std;

// 创建对象
LogUtils LogUtils;
StringUtils stringUtils;
DhtSensor DhtSensor;
PCD8544 PCD8544;
DustSensor DustSensor;
NodeMcu nodeMcu;
ESP8266WebServer server(80);

const size_t bufferSize = JSON_ARRAY_SIZE(0) + JSON_ARRAY_SIZE(1) + JSON_ARRAY_SIZE(10) + 8*JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + 2*JSON_OBJECT_SIZE(6) + 11*JSON_OBJECT_SIZE(9) + JSON_OBJECT_SIZE(13) + 1820;
DynamicJsonBuffer jsonBuffer(bufferSize);


WiFiClient client;

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
    // nodeMcu.setMode(WIFI_AP_STA);
    // nodeMcu.startSoftAP("node-weather", "12345678");
    // delay(5000);

    LogUtils.d("AP IP: ");
    Serial.println(nodeMcu.getAPIP());
    nodeMcu.connectWifi("HappyHome", "85273202");
    LogUtils.d("STA IP: ");
    Serial.println(nodeMcu.getSTAIP());

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


    // 信息
    const char* host = "api.thinkpage.cn";
    const char* url = "/v3/weather/now.json?key=24qbvr1mjsnukavo&location=shijiazhuang&language=en";


    // 连接
    LogUtils.d("start connect host");
    if (!client.connect(host, 80)) {
        LogUtils.d("host connect failed!");
    } else {
        LogUtils.d("host connected!");
    }
    delay(100);

    // 发送请求
    String request = String("GET ") + url + " HTTP/1.1\r\n"
        + "Host: " + host + "\r\n"
        + "Connection: close\r\n\r\n";

    LogUtils.d("start requesting:");
    LogUtils.d(request);
    client.print(request);
    while (client.connected()) {
        String line = client.readStringUntil('\n');

        if (line == "\r") {
            LogUtils.d("headers received: " + line);
            break;
        }
    }
    LogUtils.d("reading response...");

    String jsonStr = client.readString();
    LogUtils.d("response: " + jsonStr);
    client.stop();


    LogUtils.d("HEAP: " + String(ESP.getFreeHeap()));

    JsonObject& root = jsonBuffer.parseObject(jsonStr);

    // 心知天气
    const char* city = root["results"][0]["location"]["name"];
    const char* condition = root["results"][0]["now"]["text"];
    const char* outsideTemp = root["results"][0]["now"]["temperature"];
    LogUtils.d(root["results"][0]["location"]["name"]);
    LogUtils.d(root["results"][0]["now"]["text"]);
    LogUtils.d(root["results"][0]["now"]["temperature"]);
    
    PCD8544.clearScreen();

    PCD8544.textCenter(city, 1, true);
    PCD8544.newLine();
    PCD8544.newLine();
    PCD8544.textCenter(outsideTemp, 2, false);
    PCD8544.newLine();
    PCD8544.textCenter(condition, 1, false);
    PCD8544.show();
  
    delay(5000);

    // 获取传感器信息

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

    PCD8544.text("              ", 1, true);
    PCD8544.newLine();
    PCD8544.text(PCD8544.genString("TEMP", (int)realTemp), 1, false);
    PCD8544.newLine();
    PCD8544.text(PCD8544.genString("FEEL", (int)feelTemp), 1, false);
    PCD8544.newLine();
    PCD8544.text(PCD8544.genString("HUMID", (int)humid), 1, false);
    PCD8544.newLine();
    int intAQI = (int)avgAqi;
    if (intAQI < 0) intAQI = 0;
    PCD8544.text(PCD8544.genString("AQI", (int)intAQI), 1, false);
    PCD8544.newLine();
    // 显示IP
    String ip = nodeMcu.getAPIP().toString();
    PCD8544.text(PCD8544.genString("IP:" + ip), 1, true);
    PCD8544.show();

    delay(5000);
}


