#include "Sensor.h"

void setup() {
  Serial.begin(9600);
  // センサーの初期化
  initSensor();
}

void loop() {
  // センサーの値を読み取り
  int sensorVal = readSensor();

  // シリアルへ出力
  Serial.print("Sensor Value: ");
  Serial.println(sensorVal);

  // 10ms待機
  delay(10);
}
