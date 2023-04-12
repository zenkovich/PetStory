mkdir Build
cmake -G "Visual Studio 17 2022" -B Build

@echo off
echo Do you want to open project? (y/n)
choice /c yn /n
if errorlevel 2 goto no
    echo Opening project...
    start Build/PetStory.sln
goto end

:end