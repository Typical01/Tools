@echo off
chcp 65001
mkdir "C:\Typical\ProgramProject\C++\CMake\Tools\build"
cd /d "C:\Typical\ProgramProject\C++\CMake\Tools\build"
echo 切换到 C:\Typical\ProgramProject\C++\CMake\Tools\build 目录
cmake ..
if %errorlevel% neq 0 (
    echo 运行cmake失败 ..
    pause
    exit /b %errorlevel%
)
echo 运行cmake成功
cmake --build . --config Debug
if %errorlevel% neq 0 (
    echo 生成调试配置失败
    pause
    exit /b %errorlevel%
)
echo 生成调试配置成功
cmake --build . --config Release
if %errorlevel% neq 0 (
    echo 生成发布配置失败
    pause
    exit /b %errorlevel%
)
echo 生成发布配置成功
echo 所有操作成功完成.
pause
