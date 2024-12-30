#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import os
import sys
import shutil
import re
from pathlib import Path

# Arduino → emulatorXXXX
REPLACEMENTS = [
    # Arduino.h → mock_arduino.h
    (r'#\s*include\s*<Arduino\.h>', '#include "mock_arduino.h"'),

    # Serial
    (r'\bSerial\.begin\s*\(\s*([0-9]+)\s*\)', r'emulatorSerialBegin(\1)'),
    (r'\bSerial\.println\s*\(\s*(.*)\)', r'emulatorSerialPrintln(\1)'),
    (r'\bSerial\.print\s*\(\s*(.*)\)', r'emulatorSerialPrint(\1)'),

    # pinMode, analogRead, delay, millis
    (r'\bpinMode\s*\(\s*([^\)]+)\)', r'emulatorPinMode(\1)'),
    (r'\banalogRead\s*\(\s*([^\)]+)\)', r'emulatorAnalogRead(\1)'),
    (r'\bdelay\s*\(\s*([^\)]+)\)', r'emulatorDelay(\1)'),
    (r'\bmillis\s*\(\s*\)', r'emulatorMillis()'),
]

def transform_code(code: str) -> str:
    """
    Arduino特有の呼び出しを emulatorXXXX 系に書き換える関数
    """
    replaced = code
    for pattern, repl in REPLACEMENTS:
        replaced = re.sub(pattern, repl, replaced)
    return replaced

def main():
    """
    Usage: python convert.py <target_dir>
    例: python convert.py ../src/main
    """
    if len(sys.argv) < 2:
        print("Usage: python convert.py <target_dir>")
        sys.exit(1)

    target_dir = Path(sys.argv[1]).resolve()
    if not target_dir.is_dir():
        print(f"[ERROR] Target '{target_dir}' is not a directory.")
        sys.exit(1)

    script_dir = Path(__file__).parent.resolve()
    temp_dir = script_dir / "tempcode"

    # tempcode を再作成
    if temp_dir.exists():
        shutil.rmtree(temp_dir)
    temp_dir.mkdir(parents=True)

    print(f"[INFO] Copying and transforming code from: {target_dir}")
    print(f"[INFO] Output to: {temp_dir}")

    # 1) .ino, .cpp, .h をコピー＆置換
    patterns = ["*.ino", "*.cpp", "*.h"]
    for pat in patterns:
        for fpath in target_dir.rglob(pat):
            rel = fpath.relative_to(target_dir)
            out_path = temp_dir / rel
            out_path.parent.mkdir(parents=True, exist_ok=True)

            with open(fpath, "r", encoding="utf-8") as fr:
                content = fr.read()

            # 置換
            replaced = transform_code(content)

            # .ino → _ino.cpp
            if fpath.suffix.lower() == ".ino":
                out_path = out_path.with_name(out_path.stem + "_ino.cpp")

            # UTF-8(BOM付き)で保存
            with open(out_path, "w", encoding="utf-8-sig") as fw:
                fw.write(replaced)

            print(f"  - Transformed: {rel} -> {out_path.name}")

    # 2) mock_arduino.h & mock_arduino.cpp を tempcode にコピー
    for mock_file in ["mock_arduino.h", "mock_arduino.cpp"]:
        mock_src = script_dir / mock_file
        if mock_src.exists():
            mock_dest = temp_dir / mock_file
            with open(mock_src, "r", encoding="utf-8") as fr:
                content = fr.read()
            with open(mock_dest, "w", encoding="utf-8-sig") as fw:
                fw.write(content)
            print(f"  - Copied {mock_file} -> {mock_dest.name}")
        else:
            print(f"[WARN] {mock_file} not found in {script_dir}")

    # 3) arduino_runner.cpp を生成 (mainで setup/loop を呼び出す)
    runner_code = r'''
#include "mock_arduino.h"

// 元スケッチ(.ino等)にある setup()/loop() をextern参照
extern void setup();
extern void loop();

int main() {
    setup();
    for(int i=0; i<10; i++){
        loop();
    }
    return 0;
}
'''
    runner_path = temp_dir / "arduino_runner.cpp"
    with open(runner_path, "w", encoding="utf-8-sig") as fw:
        fw.write(runner_code)
    print(f"  - Added: arduino_runner.cpp")

    # 4) CMakeLists.txt を生成
    cmake_text = r'''cmake_minimum_required(VERSION 3.15)
project(MyArduinoProjectTest)

set(CMAKE_CXX_STANDARD 11)

file(GLOB SRCFILES "*.cpp" "*.h")

add_executable(MyArduinoProjectTest ${SRCFILES})
'''
    cmake_path = temp_dir / "CMakeLists.txt"
    with open(cmake_path, "w", encoding="utf-8-sig") as fw:
        fw.write(cmake_text)
    print(f"  - Generated: CMakeLists.txt")

    print("[INFO] Done. You can now build in 'tempcode' directory.")

if __name__ == "__main__":
    main()
