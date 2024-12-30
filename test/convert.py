#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import os
import sys
import subprocess
import platform
import shutil

def main():
    # スクリプトのあるディレクトリに移動（お好み）
    script_dir = os.path.dirname(os.path.abspath(__file__))
    os.chdir(script_dir)

    print("[INFO] === Step 1: Pythonでコード変換 ===")
    # convert.py を実行 (ターゲット: ../src/main)
    subprocess.check_call(["python", "convert.py", "../src/main"], cwd="test")

    print("[INFO] === Step 2: ビルド用ディレクトリを作りCMake実行 ===")
    build_dir = os.path.join(script_dir, "build")
    if os.path.exists(build_dir):
        shutil.rmtree(build_dir)
    os.makedirs(build_dir)
    os.chdir(build_dir)

    # --- Windows / Ubuntu の分岐 ---
    current_platform = sys.platform
    # 例:
    #  - Windows なら "win32" (または "cygwin", "msys" など)
    #  - Ubuntu なら "linux" or "linux2"
    #  - Macなら "darwin"

    if current_platform.startswith("win"):
        # Windows環境向け
        print("[INFO] Detected Windows. Using Visual Studio generator & Debug config.")
        generator = "Visual Studio 17 2022"
        build_config = "Debug"
        exe_path = os.path.join(build_dir, "Debug", "MyArduinoProjectTest.exe")
        # CMake発行
        subprocess.check_call(["cmake", "-G", generator, "../test/tempcode"])
        subprocess.check_call(["cmake", "--build", ".", "--config", build_config])

        # 実行
        if not os.path.isfile(exe_path):
            print(f"[ERROR] Executable not found: {exe_path}")
            sys.exit(1)
        print("[INFO] === Step 3: 実行ファイルを起動 ===")
        subprocess.check_call([exe_path])  # 実行

    elif current_platform.startswith("linux"):
        # Ubuntu (Linux) 想定
        print("[INFO] Detected Linux. Using Ninja generator & Release config.")
        generator = "Ninja"
        build_config = "Release"
        exe_path = os.path.join(build_dir, "MyArduinoProjectTest")

        # CMake発行
        subprocess.check_call(["cmake", "-G", generator, "-DCMAKE_BUILD_TYPE="+build_config, "../test/tempcode"])
        subprocess.check_call(["cmake", "--build", "."])

        # 実行
        if not os.path.isfile(exe_path):
            print(f"[ERROR] Executable not found: {exe_path}")
            sys.exit(1)
        print("[INFO] === Step 3: 実行ファイルを起動 ===")
        subprocess.check_call([exe_path])

    else:
        print("[ERROR] Unsupported platform:", current_platform)
        sys.exit(1)

    print("[INFO] === Done! ===")

if __name__ == "__main__":
    main()
