#include "Sensor.h"

// センサーを初期化する関数
void initSensor() {
  pinMode(A0, INPUT);  // A0ピンを入力モードに設定
}

// センサー(アナログピン)を読み取って返す関数
int readSensor() {
  // 0〜1023の範囲でアナログ値が返ってくる
  int value = analogRead(A0);
  return value;
}
