#ifndef MOCK_ARDUINO_H
#define MOCK_ARDUINO_H

#include <string>
#include <iostream>

//-------------------------------------
// Arduinoでよく使われる定数をマクロ化
//-------------------------------------
#ifndef A0
#define A0 14
#endif
#ifndef INPUT
#define INPUT 0
#endif
#ifndef OUTPUT
#define OUTPUT 1
#endif
#ifndef INPUT_PULLUP
#define INPUT_PULLUP 2
#endif

#ifndef HIGH
#define HIGH 1
#endif
#ifndef LOW
#define LOW 0
#endif

//-------------------------------------
// グローバル疑似時間
//-------------------------------------
extern unsigned long g_emulator_millis;  

//-------------------------------------
// 関数プロトタイプ
//-------------------------------------
unsigned long emulatorMillis();
void emulatorDelay(unsigned long ms);

void emulatorPinMode(int pin, int mode);
int emulatorAnalogRead(int pin);

void emulatorSerialBegin(int baud);
void emulatorSerialPrint(const std::string &s);
void emulatorSerialPrintln(const std::string &s);
void emulatorSerialPrint(int val);
void emulatorSerialPrintln(int val);

#endif // MOCK_ARDUINO_H
