@echo off
setlocal

cd src

where gcc >nul 2>nul
if %errorlevel% neq 0 (
    echo GCC (MinGW) not found. Please install MinGW or another GCC compiler and add it to your PATH.
    exit /b 1
)

gcc zrx_compiler.c -o zrx_compiler.exe
if %errorlevel% neq 0 (
    echo Compilation failed.
    exit /b 1
)

set "target_dir=%USERPROFILE%\zrx"
if not exist "%target_dir%" (
    mkdir "%target_dir%"
)

move /Y zrx_compiler.exe "%target_dir%\zrx.exe"

echo %PATH% | find /i "%target_dir%" >nul
if %errorlevel% neq 0 (
    echo Adding %target_dir% to PATH.
    setx PATH "%PATH%;%target_dir%" >nul
    if %errorlevel% neq 0 (
        echo Failed to update PATH.
        exit /b 1
    )
)

set "PATH=%PATH%;%target_dir%"

echo Installation complete! You can now use the 'zrx' command from any terminal.
echo If the 'zrx' command is not recognized, please restart your terminal.
endlocal
