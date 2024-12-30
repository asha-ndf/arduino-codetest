#!/usr/bin/env bash
set -e

echo "[INFO] === Step 1: Pythonでコード変換 ==="
cd test
python3 convert.py ../src/main

echo "[INFO] === Step 2: ビルド用ディレクトリを作りCMake実行 ==="
cd ..
rm -rf build
mkdir build
cd build

# Ninja + Release
cmake -G Ninja -DCMAKE_BUILD_TYPE=Release ../test/tempcode
cmake --build .

echo "[INFO] === Step 3: 実行ファイルを起動 ==="
./MyArduinoProjectTest

echo "[INFO] === Done! ==="
