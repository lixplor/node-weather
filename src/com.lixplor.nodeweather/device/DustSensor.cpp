#include "DustSensor.h"

GP2Y1010AU0F DustSensor::dustSensor = GP2Y1010AU0F(PIN_LED, PIN_OUTPUT);

// 存储AQI数据
vector<double> DustSensor::aqiQueue;

// 计算 ------------------------

// 采样获取输出电压
double DustSensor::getOutputV() {
    return dustSensor.getOutputV();
}

// 计算灰尘浓度
double DustSensor::calcDustDensity(double outputV) {
    return dustSensor.getDustDensity(outputV);
}

// 通过灰尘浓度计算AQI
double DustSensor::calcAQI(double dustDensity) {
    return dustSensor.getAQI(dustDensity);
}

// 获取当前AQI
double DustSensor::getCurrentAqi() {
    return dustSensor.getAQI(calcDustDensity(getOutputV()));
}

// 向数据队列中添加数据, 如果队列已满, 则删除头, 插入尾
void DustSensor::addList(double aqi) {
    if (aqiQueue.size() >= 100) {
        vector<double>::iterator i = aqiQueue.begin();
        aqiQueue.erase(i);
    }
    aqiQueue.push_back(aqi);
}

// 功能 --------------------

// 获取队列中保存的AQI平均值
double DustSensor::getAvgAQI() {
    int dataSize = aqiQueue.size();
    if (dataSize < 100) {
        return -1;
    }
    double aqiSum = 0;
    for (int i = 0; i < dataSize; i++) {
        aqiSum += aqiQueue[i];
    }
    return aqiSum / dataSize;
}

// 计算当前污染级别
int DustSensor::getAqiGrade(double aqi) {
    return dustSensor.getGradeInfo(aqi);
}

// // 检测空气, 得出一定时间内的平均aqi
// double getAvgAqi() {
//   double sumAqi = 0;
//   int count = AQI_SUM_DURATION / AQI_INTERVAL;
//   for (int i = 0; i < count; i ++) {
//     double outputV = GP2Y1010AU0F.getOutputV(); //采样获取输出电压
//     double ugm3 = GP2Y1010AU0F.getDustDensity(outputV); //计算灰尘浓度
//     double aqi = GP2Y1010AU0F.getAQI(ugm3); //计算aqi
//     int gradeInfo = GP2Y1010AU0F.getGradeInfo(aqi); //计算级别
//     //累加
//     sumAqi += aqi;
//     //计算等级
//     String grade;
//     switch (gradeInfo) {
//       case GRADE_PERFECT:
//         grade = String("GRADE_PERFECT");
//         break;
//       case GRADE_GOOD:
//         grade = String("GRADE_GOOD");
//         break;
//       case GRADE_POLLUTED_MILD:
//         grade = String("GRADE_POLLUTED_MILD");
//         break;
//       case GRADE_POLLUTED_MEDIUM:
//         grade = String("GRADE_POLLUTED_MEDIUM");
//         break;
//       case GRADE_POLLUTED_HEAVY:
//         grade = String("GRADE_POLLUTED_HEAVY");
//         break;
//       case GRADE_POLLUTED_SEVERE:
//         grade = String("GRADE_POLLUTED_SEVERE");
//         break;
//     }
//     //打印到串口
//     LogUtils.d(String("outputV=") + outputV + "\tug/m3=" + ugm3 + "\tAQI=" + aqi + "\tgrade=" + grade);
//     //间隔
//     delay(AQI_INTERVAL);
//   }
//   //统计完毕后, 计算平均aqi
//   double avgAqi = sumAqi / count;
//   return avgAqi;
// }