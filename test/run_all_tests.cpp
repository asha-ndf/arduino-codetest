#include <iostream>
#include <string>
#include <vector>
#include "mock_arduino.h"
#include "test_case_manager.h"

// グローバル定義
TestCaseManager g_manager;

// Arduino風の setup, loop があるなら extern
extern void setup();
extern void loop();

/**
 * main関数:
 *   実行時に引数で JSONファイル名を受け取り、TestCaseManagerに読み込み、
 *   setup()/loop() を走らせる簡易テスト実装。
 */
int main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <testcase.json>" << std::endl;
        return 1;
    }

    // コマンドライン引数として受け取ったケースファイル
    std::string caseFile = argv[1];
    std::cout << "[run_all_tests] Loading case: " << caseFile << std::endl;

    // JSONを読み込む
    if(!g_manager.loadFromJson(caseFile)) {
        std::cerr << "[run_all_tests] Failed to load " << caseFile << std::endl;
        return 1;
    }

    // Arduino的な setup() / loop()を呼ぶ例
    if (&setup) {
        setup();
    }

    // 100回ほどloop()してテストを進行
    for(int i = 0; i < 100; i++) {
        if (&loop) {
            loop();
        }
    }

    std::cout << "[run_all_tests] Done test for " << caseFile << std::endl;
    return 0;
}
