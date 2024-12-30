#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import os
import sys
import shutil
import re
from pathlib import Path

# Arduino -> emulatorXXXX の置換パターン
REPLACEMENTS = [
    # Arduino.h -> mock_arduino.h
    (r'#\s*include\s*<Arduino\.h>', '#include "mock_arduino.h"'),

    # Serial
    (r'\bSerial\.begin\s*\(\s*([0-9]+)\s*\)', r'emulatorSerialBegin(\1)'),
    (r'\bSerial\.println\s*\(\s*(.*)\)', r'emulatorSerialPrintln(\1)'),
    (r'\bSerial\.print\s*\(\s*(.*)\)', r'emulatorSerialPrint(\1)'),

    # pinMode, digitalRead, digitalWrite
    (r'\bpinMode\s*\(\s*([^\)]+)\)', r'emulatorPinMode(\1)'),
    (r'\bdigitalRead\s*\(\s*([^\)]+)\)', r'emulatorDigitalRead(\1)'),
    (r'\bdigitalWrite\s*\(\s*([^\)]+)\)', r'emulatorDigitalWrite(\1)'),

    # analogRead, analogWrite
    (r'\banalogRead\s*\(\s*([^\)]+)\)', r'emulatorAnalogRead(\1)'),
    (r'\banalogWrite\s*\(\s*([^\)]+)\)', r'emulatorAnalogWrite(\1)'),

    # delay, millis
    (r'\bdelay\s*\(\s*([^\)]+)\)', r'emulatorDelay(\1)'),
    (r'\bmillis\s*\(\s*\)', r'emulatorMillis()'),
]

def transform_code(code: str) -> str:
    """
    Arduino固有の呼び出しを emulatorXXXX に置換する。
    """
    replaced = code
    for pattern, repl in REPLACEMENTS:
        replaced = re.sub(pattern, repl, replaced)
    return replaced

def main():
    """
    Usage:
      python convert_allcases.py <target_dir>

    1) <target_dir> 内の .ino / .cpp / .h をコピー & 置換し、test/tempcode に配置。
    2) mock_arduino.*, test_case_manager.*, run_all_tests.cpp を同じフォルダにコピー。
    3) nlohmann ディレクトリごとコピーして #include "nlohmann/json.hpp" を使えるようにする。
    4) CMakeLists.txt を生成。
    """
    if len(sys.argv) < 2:
        print("Usage: python convert_allcases.py <target_dir>")
        sys.exit(1)

    target_dir = Path(sys.argv[1]).resolve()
    if not target_dir.is_dir():
        print(f"[ERROR] '{target_dir}' is not a directory.")
        sys.exit(1)

    script_dir = Path(__file__).parent.resolve()
    temp_dir = script_dir / "tempcode"

    # 1) tempcode を作り直す
    if temp_dir.exists():
        shutil.rmtree(temp_dir)
    temp_dir.mkdir(parents=True)

    print(f"[INFO] Converting code from {target_dir}")
    print(f"[INFO] Output to {temp_dir}")

    # 2) .ino, .cpp, .h をコピー & 置換
    exts = ["*.ino", "*.cpp", "*.h"]
    for ext in exts:
        for fpath in target_dir.rglob(ext):
            rel = fpath.relative_to(target_dir)
            out_path = temp_dir / rel
            out_path.parent.mkdir(parents=True, exist_ok=True)

            with open(fpath, "r", encoding="utf-8") as fr:
                content = fr.read()

            replaced = transform_code(content)

            # .ino => _ino.cpp
            if fpath.suffix.lower() == ".ino":
                out_path = out_path.with_name(out_path.stem + "_ino.cpp")

            # UTF-8 BOM付きで保存
            with open(out_path, "w", encoding="utf-8-sig") as fw:
                fw.write(replaced)
            print(f"  - {rel} -> {out_path.name}")

    # 3) 必要ファイルをコピー (mock_arduino.*, test_case_manager.*, run_all_tests.cpp)
    extra_files = [
        "mock_arduino.h",
        "mock_arduino.cpp",
        "test_case_manager.h",
        "test_case_manager.cpp",
        "run_all_tests.cpp",
    ]
    for ef in extra_files:
        src_path = script_dir / ef
        if src_path.exists():
            dst_path = temp_dir / ef
            with open(src_path, "r", encoding="utf-8") as fr:
                content = fr.read()
            with open(dst_path, "w", encoding="utf-8-sig") as fw:
                fw.write(content)
            print(f"  - Copied {ef} to {dst_path.name}")
        else:
            print(f"[WARN] {ef} not found in {script_dir}, skipping.")

    # 4) nlohmann ディレクトリごとコピー
    nlohmann_src = script_dir / "nlohmann"
    nlohmann_dst = temp_dir / "nlohmann"
    if nlohmann_src.is_dir():
        shutil.copytree(nlohmann_src, nlohmann_dst)
        print(f"  - Copied 'nlohmann' folder recursively.")
    else:
        print("[WARN] nlohmann directory not found, skipping.")

    # 5) CMakeLists.txt 生成
    cmake_text = r'''cmake_minimum_required(VERSION 3.15)
project(MyArduinoProjectTest)

set(CMAKE_CXX_STANDARD 11)

# すべての .cpp/.h をビルド対象に
file(GLOB SRCFILES "*.cpp" "*.h")

add_executable(run_all_tests ${SRCFILES})
'''
    cmake_path = temp_dir / "CMakeLists.txt"
    with open(cmake_path, "w", encoding="utf-8-sig") as fw:
        fw.write(cmake_text)
    print(f"  - Generated CMakeLists.txt")

    print("[INFO] Done. You can now build in 'tempcode' directory.")

if __name__ == "__main__":
    main()
