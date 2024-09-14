@echo off

set BUILD_DIR=cmbuild

if not exist %BUILD_DIR% (
    mkdir %BUILD_DIR%
) 

cd %BUILD_DIR%
cmake ..
if %ERRORLEVEL% neq 0 (
    @echo cmake failed
    cd ..
    exit /b
)

cmake --build . -- /p:Configuration=Release
if %ERRORLEVEL% neq 0 (
    @echo build failed
    cd ..
    exit /b
)
cd ..

@echo build ok! 

echo.

cd /d %~dp0
%BUILD_DIR%\Release\log_test.exe
echo.
