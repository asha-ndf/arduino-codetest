#ifndef MOCK_ARDUINO_H
#define MOCK_ARDUINO_H

#include <string>
#include <iostream>

/* 定数類 */
#ifndef A0
#define A0 14
#endif
#ifndef INPUT
#define INPUT 0
#endif
#ifndef OUTPUT
#define OUTPUT 1
#endif
#ifndef HIGH
#define HIGH 1
#endif
#ifndef LOW
#define LOW 0
#endif

// グローバル疑似時間
extern unsigned long g_mock_millis;

// プロトタイプ宣言
unsigned long myMillis();
void myDelay(unsigned long ms);
void myPinMode(int pin, int mode);
int myAnalogRead(int pin);

void mySerialBegin(int baud);
void mySerialPrint(const std::string &s);
void mySerialPrintln(const std::string &s);
void mySerialPrint(int val);
void mySerialPrintln(int val);

#endif // MOCK_ARDUINO_H
