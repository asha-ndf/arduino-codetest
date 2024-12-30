#!/usr/bin/env bash
set -e

# このファイルがあるフォルダ(test)へ移動
cd "$(dirname "$0")"

echo "[INFO] === Step 1: Pythonでコード変換 ==="
python convert.py ../src/main

echo "[INFO] === Step 2: ビルド用ディレクトリを作りCMake実行 ==="
cd ..
rm -rf build
mkdir build
cd build
cmake -G "Visual Studio 17 2022" ../test/tempcode
cmake --build . --config Debug

echo "[INFO] === Step 3: 実行ファイルを起動 ==="
./Debug/MyArduinoProjectTest.exe

echo "[INFO] === Done! ==="
