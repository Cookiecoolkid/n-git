#!/bin/bash

# 初始化测试环境
rm -r test_repo
mkdir test_repo
cd test_repo || exit

# 初始化 Nit 仓库
../nit init
if [ ! -d ".nit" ]; then
    echo "testInit failed."
    exit 1
fi
echo "testInit passed."

# 创建并添加文件 a.txt
touch a.txt
../nit add a.txt
if [ ! -f ".nit/index" ]; then
    echo "testAdd a.txt failed."
    exit 1
fi

echo "testAdd a.txt passed."

# 修改 a.txt 并添加
echo "hello" > a.txt
../nit add a.txt
if [ ! -f ".nit/index" ]; then
    echo "testAdd modified a.txt failed."
    exit 1
fi
echo "testAdd modified a.txt passed."

# 删除 a.txt 并添加
rm a.txt
../nit add a.txt
if [ ! -f ".nit/index" ]; then
    echo "testAdd removed a.txt failed."
    exit 1
fi
echo "testAdd removed a.txt passed."

# 创建并添加文件 b.txt
touch b.txt
echo "world" > b.txt
../nit add b.txt
if [ ! -f ".nit/index" ]; then
    echo "testAdd b.txt failed."
    exit 1
fi
echo "testAdd b.txt passed."

# 提交更改
../nit commit "commit b.txt"
if [ ! -f ".nit/HEAD" ]; then
    echo "testCommit failed."
    exit 1
fi
echo "testCommit passed."

# 查看日志
../nit log
if [ $? -ne 0 ]; then
    echo "testLog failed."
    exit 1
fi
echo "testLog passed."

echo "All tests passed."
