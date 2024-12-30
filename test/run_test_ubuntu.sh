#!/usr/bin/env bash
set -e

# このスクリプトファイルが置いてあるフォルダ(test)に移動
cd "$(dirname "$0")"

echo "[INFO] === Step 1: Pythonでコード変換 ==="
# convert.py を実行。ターゲットディレクトリ: ../src/main
python convert.py ../src/main

echo "[INFO] === Step 2: ビルド用ディレクトリを作りCMake実行 ==="
cd ..
rm -rf build
mkdir build
cd build

# Ninjaを使ったReleaseビルド
cmake -G "Ninja" -DCMAKE_BUILD_TYPE=Release ../test/tempcode
cmake --build .

echo "[INFO] === Step 3: 実行ファイルを起動 ==="
# Ninjaの場合、同じフォルダ(build/)に実行ファイルが配置される
./MyArduinoProjectTest

echo "[INFO] === Done! ==="
