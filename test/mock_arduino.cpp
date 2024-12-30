#include "mock_arduino.h"
#include <iostream>
#include <string>

// TestCaseManager の参照
#include "test_case_manager.h"
extern TestCaseManager g_manager;  // run_all_tests.cpp 等で実体定義している

//-------------------------------------
// グローバル変数の実体(1回だけ)
//-------------------------------------
unsigned long g_emulator_millis = 0;

//-------------------------------------
// 時間管理
//-------------------------------------
unsigned long emulatorMillis() {
    return g_emulator_millis;
}

void emulatorDelay(unsigned long ms) {
    // 実際には待たずに疑似時刻を進める
    g_emulator_millis += ms;
}

//-------------------------------------
// デジタルIO
//-------------------------------------
void emulatorPinMode(int pin, int mode) {
    // 必要があればログ出力など。ここでは何もしない。
}

int emulatorDigitalRead(int pin) {
    // TestCaseManager から (pin, g_emulator_millis) に対応するデジタル値(0 or 1)を取得
    int val = g_manager.getDigitalValue(pin, g_emulator_millis);
    return val; // 0=LOW, 1=HIGH
}

void emulatorDigitalWrite(int pin, int value) {
    // テストのデバッグ用にログを出す例
    std::cout << "[EMULATOR] digitalWrite(pin=" << pin
              << ", val=" << value
              << ") at " << g_emulator_millis << "ms\n";
}

//-------------------------------------
// アナログIO
//-------------------------------------
int emulatorAnalogRead(int pin) {
    // TestCaseManager から (pin, g_emulator_millis) に対応するアナログ値(0〜1023)を取得
    int val = g_manager.getAnalogValue(pin, g_emulator_millis);
    return val;
}

void emulatorAnalogWrite(int pin, int value) {
    // PWMなどのログを取りたい場合に出力する例
    std::cout << "[EMULATOR] analogWrite(pin=" << pin
              << ", duty=" << value
              << ") at " << g_emulator_millis << "ms\n";
}

//-------------------------------------
// シリアル
//-------------------------------------
void emulatorSerialBegin(int baud) {
    // シリアル初期化のモック
    std::cout << "[EMULATOR] Serial.begin(" << baud
              << ") at " << g_emulator_millis << "ms\n";
}

void emulatorSerialPrint(const std::string &s) {
    std::cout << s;
    // 必要に応じてログバッファに保存し、
    // テスト結果と比較しても良い
}

void emulatorSerialPrintln(const std::string &s) {
    std::cout << s << std::endl;
}

void emulatorSerialPrint(int val) {
    std::cout << val;
}

void emulatorSerialPrintln(int val) {
    std::cout << val << std::endl;
}
