# Arduino Code Testing Project

## 以下、ChatGPT によって生成された要約文です

このリポジトリでは、Arduino の `.ino` や `.cpp` コードを PC 上でモックし、複数のテストケース（JSON）を用いて実行する仕組みを構築しています。  
GitHub Actions を用いた自動テストにも対応し、複数ファイルに分割された Arduino スケッチを一括テストできます。

---

## 主な特徴

1. **Arduino 特有の API (`pinMode`, `digitalWrite`, `analogRead` 等) をモック実装**
   - `mock_arduino.*` によって擬似的な Arduino 関数が定義され、`g_emulator_millis` を疑似時刻として管理。
2. **`TestCaseManager` によるテストケース管理**
   - `timeValues` (アナログ入力: ピン+時刻+値)
   - `digitalValues` (デジタル入力: ピン+時刻+HIGH/LOW)
   - JSON ファイルで定義し、Zero-Order Hold 等のロジックで `timeMs` が進むほど入力値を変化させます。
3. **`run_all_tests.cpp` でメイン関数(エントリポイント)を定義**
   - Arduino コードの `setup()` / `loop()` を呼ぶ。
   - 引数で JSON ファイルを受け取りテストを実行。あるいは Bash スクリプトで複数ファイルを一括実行。
4. **GitHub Actions 上で自動テスト**
   - `.github/workflows/test-ubuntu.yml` により、Ninja ビルド後、`testcases/` の `.json` を一括実行し、`k/n` の形式でサマリを表示 (すべて合格としてカウント) 。

---

## ディレクトリ構成例

```plaintext
arduino-codetest/
 ├─ src/
 │   └─ main/
 │       ├─ main.ino
 │       ├─ Sensor.cpp
 │       └─ Sensor.h
 ├─ test/
 │   ├─ convert_allcases.py
 │   ├─ mock_arduino.h
 │   ├─ mock_arduino.cpp
 │   ├─ test_case_manager.h
 │   ├─ test_case_manager.cpp
 │   ├─ run_all_tests.cpp
 │   ├─ run_alltestcases_win.sh
 │   ├─ run_testcases_ubuntu.sh
 │   ├─ testcases/
 │   │   ├─ case1.json
 │   │   ├─ case2.json
 │   │   └─ ...
 │   └─ tempcode/
 │       ├─ ... (Pythonスクリプト実行後に生成される)
 ├─ .github/
 │   └─ workflows/
 │       └─ test-ubuntu.yml
 └─ build/
     ├─ Debug/ (Windowsビルド成果物)
     └─ run_all_tests.exe (生成物)
```

- `convert_allcases.py` (または `convert.py`) は Arduino スケッチをモック用に置換し、 `tempcode/` に C++コードや `CMakeLists.txt` を生成。
- `mock_arduino.*` が Arduino API をモック化 (`g_emulator_millis`, `emulatorAnalogRead`, `emulatorDigitalRead` など)。
- `test_case_manager.*` が JSON を読み込み、(time, pin)に応じた入力値を返す。
- `run_all_tests.cpp` にメイン関数があり、コマンドライン引数で JSON ファイルを受け取ってテスト実行するか、あるいは引数なしで特定ファイルを読み込むなど自由に実装できる。

---

## 使用手順

1. **リポジトリを clone**

   ```bash
   git clone <repository_url>
   ```

2. **(ローカル) Python スクリプトを実行し、モック用コード生成**

   ```bash
   cd test
   python convert_allcases.py ../src/main
   ```

   - `tempcode/` に に変換後のソースコード & `CMakeLists.txt` 生成

3. **(ローカル) Windows でビルド (Visual Studio)**

   - `run_alltestcases_win.sh` を実行 (Git Bash 推奨) → Visual Studio ビルド → `Debug/run_all_tests.exe`
   - `testcases/` をコピー & 各 `.json` を引数に順番に実行

4. **(CI) GitHub Actions**

   - `.github/workflows/test-ubuntu.yml` が動き、`run_testcases_ubuntu.sh` を使用して Ninja ビルド & 全 `.json` 実行
   - Actions の Job Summary に「Passed k / n testcases」の形式で表示
