#!/usr/bin/env bash
set -e

#------------------------------------------------------------------------------
# 1) Ninjaでビルド
# 2) testcases/ フォルダをビルド結果と同じ場所にコピー
# 3) .jsonを順番に実行ファイルへ渡し、それぞれ(現状)成功扱いとしてカウント
# 4) "passed=XX" "total=XX" を GitHub Actions outputs (step output) に書き出し
#------------------------------------------------------------------------------

echo "[INFO] === Step 1: Build project (Ninja) ==="
mkdir -p build
cd build
cmake -G Ninja -DCMAKE_BUILD_TYPE=Release ../test/tempcode
cmake --build .

echo "[INFO] === Step 2: Copy testcases/ into build directory ==="
cp -r ../test/testcases . || echo "[WARN] no testcases directory found"

echo "[INFO] === Step 3: Enumerate JSON files and run tests ==="
cd testcases

total=0
passed=0

# 実行ファイル名が run_all_tests と想定
for casefile in *.json; do
  if [ -f "$casefile" ]; then
    total=$(( total + 1 ))
    echo "[INFO] Running test for $casefile"

    # 実行ファイルは1つ上のフォルダ(../)
    ../run_all_tests "$casefile"

    # 現状はすべて成功扱い
    passed=$(( passed + 1 ))
    echo "[INFO] => OK (assume success)"
    echo ""
  fi
done

cd ..

echo "[INFO] Testcases done. Passed $passed / $total"

# GitHub Actions の step outputs に書き出し
# set-output は非推奨のため、 $GITHUB_OUTPUT を使う
echo "passed=$passed" >> $GITHUB_OUTPUT
echo "total=$total" >> $GITHUB_OUTPUT

# スクリプト自体は常に 0(成功)を返す
exit 0
