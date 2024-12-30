#ifndef TEST_CASE_MANAGER_H
#define TEST_CASE_MANAGER_H

#include <string>
#include <vector>

//-------------------------------------
// 単純な構造体例: アナログ/デジタル入力を時刻ごとに格納
//-------------------------------------
struct AnalogInput {
    unsigned long timeMs;  // 時刻 (ミリ秒)
    int pin;               // ピン番号
    int value;             // 0〜1023程度
};

struct DigitalInput {
    unsigned long timeMs;
    int pin;
    int value;             // 0=LOW,1=HIGH
};

//-------------------------------------
// テストケースマネージャ (JSONで時刻別入力値や期待結果を管理)
//-------------------------------------
class TestCaseManager
{
public:
    // JSONファイルの読み込み
    bool loadFromJson(const std::string& jsonPath);

    // 時刻timeMs, pinに応じてアナログ値 (0〜1023)を返す
    int getAnalogValue(int pin, unsigned long timeMs);

    // デジタル値 (0/1)を返す
    int getDigitalValue(int pin, unsigned long timeMs);

    // テストケース名や説明を保持
    std::string getTestCaseName() const { return testCaseName; }
    std::string getDescription() const  { return description; }

    // 期待するシリアル出力など
    const std::vector<std::string>& getExpectedOutputs() const {
        return expectedOutputs;
    }

private:
    // アナログ入力リスト
    std::vector<AnalogInput> analogInputs;
    // デジタル入力リスト
    std::vector<DigitalInput> digitalInputs;

    // テストケース名
    std::string testCaseName;
    // 説明
    std::string description;

    // 期待するシリアル出力や結果
    std::vector<std::string> expectedOutputs;
};

#endif // TEST_CASE_MANAGER_H
