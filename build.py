import os
import sys
import shutil
import subprocess

BUILD_TYPE = "Release"

def run(cmd, cwd=None):
    """执行命令并支持指定工作目录"""
    print(f"\033[32m$ {cmd}\033[0m")
    result = subprocess.run(cmd, shell=True, cwd=cwd)
    if result.returncode != 0:
        sys.exit(result.returncode)

def clean():
    if os.path.exists("build"):
        shutil.rmtree("build")
        print("Cleaned build directory.")

def main():
    global BUILD_TYPE
    args = sys.argv[1:]
    if "clean" in args:
        clean()
        return
    if "debug" in args:
        BUILD_TYPE = "Debug"
    elif "release" in args:
        BUILD_TYPE = "Release"

    # 确保 build 目录存在
    os.makedirs("build", exist_ok=True)

    # 安装依赖（在项目根目录执行）
    run("conan install . --output-folder=build --build=missing")

    # 在 build 目录下执行 CMake 和构建
    run(f"cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE={BUILD_TYPE}", 
        cwd="build")
    run("cmake --build .", cwd="build")

if __name__ == "__main__":
    main()