#include "DhtSensor.h"
using namespace std;

DHT DhtSensor::dht(PIN_DHT, DHTTYPE);

// 获取单例
DHT DhtSensor::getDHTInstance() {
    DHT *pDHT = &DhtSensor::dht;
    if (pDHT == NULL) {
        pDHT = new DHT(PIN_DHT, DHTTYPE);
    }
    return DhtSensor::dht;
}

// 转换---------------

// 摄氏温度度转化为华氏温度
double DhtSensor::Fahrenheit(double celsius) {
    return 1.8 * celsius + 32;
}

// 摄氏温度转化为开氏温度
double DhtSensor::Kelvin(double celsius) {
    return celsius + 273.15;
}

// 露点（点在此温度时，空气饱和并产生露珠）参考: http://wahiduddin.net/calc/density_algorithms.htm
double DhtSensor::dewPoint(double celsius, double humidity) {
    double A0 = 373.15 / (273.15 + celsius);
    double SUM = -7.90298 * (A0 - 1);
    SUM += 5.02808 * log10(A0);
    SUM += -1.3816e-7 * (pow(10, (11.344 * (1 - 1 / A0))) - 1) ;
    SUM += 8.1328e-3 * (pow(10, (-3.49149 * (A0 - 1))) - 1) ;
    SUM += log10(1013.246);
    double VP = pow(10, SUM - 3) * humidity;
    double T = log(VP / 0.61078); // temp var
    return (241.88 * T) / (17.558 - T);
}

// 快速计算露点，速度是5倍dewPoint() 参考: http://en.wikipedia.org/wiki/Dew_point
double DhtSensor::dewPointFast(double celsius, double humidity) {
    double a = 17.271;
    double b = 237.7;
    double temp = (a * celsius) / (b + celsius) + log(humidity / 100);
    double Td = (b * temp) / (a - temp);
    return Td;
}

// 设备控制 ------------

// 启动
void DhtSensor::begin() {
    getDHTInstance().begin();
}

// 读取湿度, 可能需要2秒
float DhtSensor::getHumid() {
    return getDHTInstance().readHumidity();
}

// 读取实际温度
float DhtSensor::getRealTemp() {
    return getDHTInstance().readTemperature();
}

// 计算体感温度
float DhtSensor::getFeelTemp(float realTemp, float humid) {
    //计算摄氏度单位的热指数 (isFahreheit = false)
    return getDHTInstance().computeHeatIndex(realTemp, humid, false);
}

// 计算结露温度
float DhtSensor::getDewPoint(float realTemp, float humid) {
    return dewPointFast(realTemp, humid);
}

