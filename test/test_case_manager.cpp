#include "test_case_manager.h"
#include <fstream>
#include <iostream>
#include <algorithm>

// 単一ヘッダの nlohmann/json
#include "nlohmann/json.hpp"
using json = nlohmann::json;

bool TestCaseManager::loadFromJson(const std::string& jsonPath)
{
    // いったんリセット
    analogInputs.clear();
    digitalInputs.clear();
    testCaseName.clear();
    description.clear();
    expectedOutputs.clear();

    std::ifstream ifs(jsonPath);
    if(!ifs.is_open()) {
        std::cerr << "[TestCaseManager] Cannot open: " << jsonPath << std::endl;
        return false;
    }

    json j;
    try {
        ifs >> j;  // JSONをパース
    }
    catch(std::exception& e) {
        std::cerr << "[TestCaseManager] JSON parse error: " << e.what() << std::endl;
        return false;
    }

    // テストケース名
    if(j.contains("testCaseName") && j["testCaseName"].is_string()) {
        testCaseName = j["testCaseName"].get<std::string>();
    }
    // 説明
    if(j.contains("description") && j["description"].is_string()) {
        description = j["description"].get<std::string>();
    }

    // アナログ入力
    if(j.contains("timeValues") && j["timeValues"].is_array()) {
        for(auto& item : j["timeValues"]) {
            AnalogInput ai;
            ai.timeMs = item.value("time", 0UL);
            ai.pin    = item.value("pin", 0);
            ai.value  = item.value("value", 0);
            analogInputs.push_back(ai);
        }
    }

    // デジタル入力
    if(j.contains("digitalValues") && j["digitalValues"].is_array()) {
        for(auto& item : j["digitalValues"]) {
            DigitalInput di;
            di.timeMs = item.value("time", 0UL);
            di.pin    = item.value("pin", 0);
            di.value  = item.value("value", 0); // 0 or 1
            digitalInputs.push_back(di);
        }
    }

    // 期待出力
    if(j.contains("expectedOutputs") && j["expectedOutputs"].is_array()) {
        for(auto& s : j["expectedOutputs"]) {
            if(s.is_string()) {
                expectedOutputs.push_back(s.get<std::string>());
            }
        }
    }

    std::cout << "[TestCaseManager] Loaded: " << testCaseName
              << " (" << description << ")" << std::endl;
    return true;
}

int TestCaseManager::getAnalogValue(int pin, unsigned long timeMs)
{
    // Zero-Order Hold: timeMs以下の最大のtimeを探す
    int ret = 0;
    unsigned long bestTime = 0;
    bool found = false;

    for(const auto& ai : analogInputs) {

        // 条件: (対象ピン) かつ (timeMs以下) かつ (最も大きいtimeMs)
        if (ai.pin == pin && ai.timeMs <= timeMs && ai.timeMs >= bestTime) {
            found = true;
            ret = ai.value;
            bestTime = ai.timeMs;
        }
    }

    if (!found) {
        // 走査が終了しても該当エントリが無い場合の警告
        std::cerr << "[WARN] getAnalogValue: No matching entry found for pin=" 
                  << pin << ", time=" << timeMs 
                  << ". Returning 0." << std::endl;
    }

    return ret;
}


int TestCaseManager::getDigitalValue(int pin, unsigned long timeMs)
{
    // 同様に timeMs以下の最大のtimeを探し、0/1を返す
    int ret = 0;   // デフォルトLOW
    unsigned long bestTime = 0;
    for(const auto& di : digitalInputs) {
        if(di.pin == pin && di.timeMs <= timeMs && di.timeMs >= bestTime) {
            ret = di.value;
            bestTime = di.timeMs;
        }
    }
    return ret;
}
