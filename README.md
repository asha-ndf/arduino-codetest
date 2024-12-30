# Arduino Code Testing Project

このリポジトリは、Arduino スケッチ( .ino ) および関連の .cpp / .h ファイルを PC 上でモックテストし、GitHub Actions を利用して自動テストを行うサンプルです。

## プロジェクト概要

- `src/main/` ディレクトリに Arduino のスケッチや .cpp / .h が置かれます。
- `test/convert.py` は、Arduino 特有の API (pinMode, analogRead, Serial.print など) をモック関数に書き換え、CMake でビルドできる C++コードへ変換します。
- `test/mock_arduino.h` & `mock_arduino.cpp` が、Arduino ハードウェア呼び出しをモック化した実装を提供します。
  - `A0`, `INPUT` などの定数、`myPinMode`, `myAnalogRead` などの関数が定義され、実機依存コードをエミュレートします。
- `test/run_test_windows.sh` では Windows + Visual Studio 用のビルドを行い、
  - `cmake -G "Visual Studio 17 2022" & --config Debug`
  - `./Debug/MyArduinoProjectTest.exe` 実行
- `test/run_test_ubuntu.sh` では Ubuntu + Ninja を使ったビルドを行い、
  - `cmake -G Ninja -DCMAKE_BUILD_TYPE=Release`
  - `./MyArduinoProjectTest` 実行
- GitHub Actions (`.github/workflows/test-ubuntu.yml` など) により、Push や Pull Request 時に自動テストを行います。

## ディレクトリ構成

```plaintext
arduino-codetest/
 ├─ src/
 │   └─ main/
 │       ├─ main.ino          # メインのArduinoスケッチ
 │       ├─ Sensor.cpp        # 追加のC++コード
 │       └─ Sensor.h          # ヘッダ
 ├─ test/
 │   ├─ convert.py            # 変換スクリプト (Arduino -> モックC++)
 │   ├─ mock_arduino.h        # モックの宣言のみ
 │   ├─ mock_arduino.cpp      # モックの実装 (実体)
 │   ├─ run_test_windows.sh   # Windows環境でのビルド＆実行
 │   ├─ run_test_ubuntu.sh    # Ubuntu環境でのビルド＆実行
 │   └─ tempcode/             # スクリプト実行時に自動生成される中間コード& CMakeLists
 ├─ build/                    # CMakeビルド用のディレクトリ (生成物など)
 │   └─ Debug/                # Windows(VS)のDebugビルド成果物 (exeなど)
 ├─ .github/
 │   └─ workflows/
 │       └─ test-ubuntu.yml   # GitHub ActionsのUbuntu CI定義例
 ├─ .gitignore                # ビルド成果物やtempcodeを除外
 └─ README.md                 # このファイル
```

## 主なファイルの説明

- `src/main/main.ino`, `Sensor.cpp`, `Sensor.h`
  - Arduino 向けに書かれたスケッチ＆追加ライブラリ/コード。
  - 例: `main.ino` では `setup()` / `loop()` を定義、`Sensor.cpp` ではセンサー読み取りなど。
- `test/convert.py`
  - Python スクリプト。
  - `.ino` / `.cpp` / `.h` から Arduino ハードウェア依存コードを正規表現で置換し、PC 上でコンパイル可能な C++へ変換して `test/tempcode/` に出力。
  - `#include <Arduino.h>` → `#include "mock_arduino.h"`
  - `pinMode` → `myPinMode` など
- `test/mock_arduino.h` / `test/mock_arduino.cpp`
  - Arduino API を模倣するモック実装。
  - `A0`, `INPUT`, `HIGH` などの定数をマクロ化し、`myPinMode`, `mySerialPrint`, `myAnalogRead` などの関数を定義。
  - 実機の代わりに適当な値を返し、PC 上でテスト可能に。
- `test/run_test_windows.sh`
  - Git Bash などで実行し、Windows + Visual Studio 用にビルド → 実行を行うサンプル。
  - `cmake -G "Visual Studio 17 2022" ...`
  - `cmake --build . --config Debug`
  - `./Debug/MyArduinoProjectTest.exe`
- `test/run_test_ubuntu.sh`
  - Ubuntu 上で実行し、Ninja を使用したビルド → 実行を行うサンプル。
  - `cmake -G Ninja -DCMAKE_BUILD_TYPE=Release ...`
  - `./MyArduinoProjectTest`
- `.github/workflows/test-ubuntu.yml`
  - GitHub Actions のワークフロー定義。
  - `ubuntu-latest` を使用し、`run_test_ubuntu.sh` を自動で実行する。
  - Push / PR 時に CI を回してテスト結果を確認できる。

## 動かし方 (ローカル)

### Windows (Visual Studio)

1. Git Bash (または WSL) を起動
2. `bash test/run_test_windows.sh`
3. `convert.py` により `test/tempcode/` に変換済みコード & CMakeLists が生成される
4. `build/` ディレクトリで Visual Studio ビルド → `Debug/MyArduinoProjectTest.exe` 実行
5. 結果がコンソールに表示される

### Ubuntu

1. `chmod +x test/run_test_ubuntu.sh`
2. `./test/run_test_ubuntu.sh`
3. `tempcode` にコードを配置
4. Ninja + Release ビルド → `MyArduinoProjectTest` 実行
5. コンソール出力を確認

## GitHub Actions での動作

- `.github/workflows/test-ubuntu.yml` があり、push や pull_request がトリガーとなって Ubuntu の仮想環境で `run_test_ubuntu.sh` を実行します。
- `convert.py` → `cmake` → `ninja` → `./MyArduinoProjectTest` の流れが自動的に行われ、結果が Actions のステータスに反映されます。
- Visual Studio 用の CI が必要な場合は、別途 `windows-latest` を指定したワークフローを用意するか、`setup-msbuild` などを利用して MSBuild を使う例もあります。
