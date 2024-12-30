#ifndef MOCK_ARDUINO_H
#define MOCK_ARDUINO_H

#include <string>

//-------------------------------------
// Arduinoで使われる定数をマクロ化
//-------------------------------------
#ifndef A0
#define A0 14
#endif
#ifndef A1
#define A1 15
#endif
// 必要なら A2, A3, etc...

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
// グローバル疑似ミリ秒 (宣言だけ)
//-------------------------------------
extern unsigned long g_emulator_millis;

//-------------------------------------
// エミュレータ関数群 (宣言のみ)
//-------------------------------------

// 時間管理
unsigned long emulatorMillis();
void emulatorDelay(unsigned long ms);

// デジタルIO
void emulatorPinMode(int pin, int mode);
int emulatorDigitalRead(int pin);
void emulatorDigitalWrite(int pin, int value);

// アナログIO
int emulatorAnalogRead(int pin);
void emulatorAnalogWrite(int pin, int value);

// シリアル
void emulatorSerialBegin(int baud);
void emulatorSerialPrint(const std::string &s);
void emulatorSerialPrintln(const std::string &s);
void emulatorSerialPrint(int val);
void emulatorSerialPrintln(int val);

#endif // MOCK_ARDUINO_H
