#!/bin/bash

# clang-tidy 自动检查脚本
# 用法：./run_clang_tidy.sh <源文件> [--fix]

# 检查参数
if [ $# -lt 1 ]; then
    echo -e "\033[31m错误：请指定要检查的源文件\033[0m"
    echo "示例: $0 main.cpp [--fix]"
    exit 1
fi

# 获取文件路径和参数
SOURCE_FILE=$1
FIX_MODE=${2:-""}

# 检查文件是否存在
if [ ! -f "$SOURCE_FILE" ]; then
    echo -e "\033[31m错误：文件 '$SOURCE_FILE' 不存在\033[0m"
    exit 1
fi

# 配置检查规则 (可根据需要修改)
CHECKS_CONFIG="
    -clang-analyzer-*,
    -modernize-*,
    -performance-*,
    -readability-*,
    -portability-*,
    -cppcoreguidelines-*
"

# 查找 clang-tidy 
CLANG_TIDY=$(which clang-tidy)
if [ -z "$CLANG_TIDY" ]; then
    echo -e "\033[31m错误：未找到 clang-tidy，请先安装\033[0m"
    echo "Ubuntu/Debian: sudo apt-get install clang-tidy"
    echo "MacOS: brew install llvm"
    exit 1
fi

# 生成临时编译数据库
echo "[" > compile_commands.json
echo "  {" >> compile_commands.json
echo "    \"directory\": \"$(pwd)\"," >> compile_commands.json
echo "    \"file\": \"$SOURCE_FILE\"," >> compile_commands.json
echo "    \"command\": \"c++ -std=c++17 -I./include $SOURCE_FILE\"" >> compile_commands.json
echo "  }" >> compile_commands.json
echo "]" >> compile_commands.json

# 运行检查
echo -e "\033[34m正在检查 $SOURCE_FILE ...\033[0m"
if [ "$FIX_MODE" == "--fix" ]; then
    $CLANG_TIDY $SOURCE_FILE -fix -checks="$CHECKS_CONFIG" -- \
        -std=c++17 -I./include 2>&1 | grep -v "warnings generated"
    echo -e "\033[32m已尝试自动修复问题 (需手动验证)\033[0m"
else
    $CLANG_TIDY $SOURCE_FILE -checks="$CHECKS_CONFIG" -- \
        -std=c++17 -I./include 2>&1 | grep -v "warnings generated" | \
        awk '{ 
            if (/warning:/) print "\033[33m" $0 "\033[0m"; 
            else if (/error:/) print "\033[31m" $0 "\033[0m"; 
            else print $0 
        }'
fi

# 清理临时文件
rm -f compile_commands.json

echo -e "\033[34m检查完成\033[0m"