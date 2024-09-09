﻿
@echo off
chcp 65001
setlocal EnableDelayedExpansion
if exist "build" rmdir /s /q "build"
mkdir "build"
cd /d "build"
echo 切换到 C:\Users\22793\source\repos\Tools\build 目录
where cmake >nul 2>&1
if %errorlevel% neq 0 (
    echo 未安装 cmake 或 cmake 不在系统 PATH 中
    pause
    exit /b 1
)
cmake ..
if %errorlevel% neq 0 (
    echo 运行 cmake 失败 ..
    pause
    exit /b %errorlevel%
)
echo 运行 cmake 成功
cmake --build . --config Debug
if %errorlevel% neq 0 (
    echo 生成 Debug 配置失败
    pause
    exit /b %errorlevel%
)
echo 生成 Debug 配置成功
cmake --build . --config Release
if %errorlevel% neq 0 (
    echo 生成 Release 配置失败
    pause
    exit /b %errorlevel%
)
echo 生成 Release 配置成功
echo 所有操作成功完成.
pause
