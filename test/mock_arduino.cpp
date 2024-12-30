#include "mock_arduino.h"

unsigned long g_mock_millis = 0;

unsigned long myMillis() {
    return g_mock_millis;
}

void myDelay(unsigned long ms) {
    g_mock_millis += ms;
}

void myPinMode(int pin, int mode) {
    // do nothing
}

int myAnalogRead(int pin) {
    return pin + (g_mock_millis % 1024);
}

void mySerialBegin(int baud) {
    std::cout << "[MOCK] Serial.begin(" << baud << ")\n";
}

void mySerialPrint(const std::string &s) {
    std::cout << s;
}
void mySerialPrintln(const std::string &s) {
    std::cout << s << std::endl;
}

void mySerialPrint(int val) {
    std::cout << val;
}
void mySerialPrintln(int val) {
    std::cout << val << std::endl;
}
