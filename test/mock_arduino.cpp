#include "mock_arduino.h"

unsigned long g_emulator_millis = 0; // 実体をここで定義

unsigned long emulatorMillis() {
    return g_emulator_millis;
}

void emulatorDelay(unsigned long ms) {
    g_emulator_millis += ms;
}

void emulatorPinMode(int pin, int mode) {
    // モック → 何もしない
}

int emulatorAnalogRead(int pin) {
    // 適当なダミー値
    return pin + (g_emulator_millis % 1024);
}

void emulatorSerialBegin(int baud) {
    std::cout << "[EMULATOR] Serial.begin(" << baud << ")\n";
}

void emulatorSerialPrint(const std::string &s) {
    std::cout << s;
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
