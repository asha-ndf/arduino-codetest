#!/usr/bin/env bash
set -e

#------------------------------------------------------------------------------
# Windows + Git Bash 向け
# Visual Studio 17 2022 でビルドし、testcases/*.json を列挙して順番に実行
#------------------------------------------------------------------------------

# このスクリプトがあるディレクトリ (test/) に移動
cd "$(dirname "$0")"

echo "[INFO] === Step 1: Pythonでコード変換 ==="
# 必要に応じて convert_allcases.py
python convert_allcases.py ../src/main

echo "[INFO] === Step 2: CMakeビルドディレクトリを作りVisual Studioビルド ==="
cd ..
rm -rf build
mkdir build
cd build
cmake -G "Visual Studio 17 2022" -A x64 ../test/tempcode
cmake --build . --config Debug

echo "[INFO] === Step 3: testcases/ をコピー ==="
# testcases/ フォルダを build/Debug にコピー (実行ファイルと同じ場所)
cp -r ../test/testcases ./Debug/testcases || echo "[WARN] no testcases dir"

echo "[INFO] === Step 4: testcases/*.json を列挙し、run_all_tests.exe <filename> で実行 ==="
cd Debug

# ここで *.json をすべて走査
for casefile in testcases/*.json; do
    if [ -f "$casefile" ]; then
        echo "[INFO] Running test for $casefile"
        ./run_all_tests.exe "$casefile"
        echo ""
    fi
done

echo "[INFO] === Done! ==="
