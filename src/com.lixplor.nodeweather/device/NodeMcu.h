#ifndef NodeMcn_h
#define NodeMcn_h

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

class NodeMcu {
    public:
        static String HTML_FRONT;
        static String HTML_AP_NAME_FRONT;
        static String HTML_AP_NAME_END;
        static String HTML_END;
        static int wifiStatus;
        void setMode(WiFiMode_t mode);
        void startSoftAP(const char* ssid, const char* password);
        bool connectWifi(char* ssid, char* password);
        void disconnectWifi();
        int scanAPNum();
        int getConnectionState();
        IPAddress getAPIP();
        IPAddress getSTAIP();
        byte* getMac();
        String getWifiSSID();
        long getWifiRSSI();
        // byte getWifiEncType();

        WiFiServer createServer(uint16_t port);
        WiFiClient listenToClient(WiFiServer server);
        char readClient(WiFiClient client);
        void response(WiFiClient client, String content);
        void closeConnection(WiFiClient client);
        bool isClientConnected(WiFiClient client);
};

#endif