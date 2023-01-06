pushd ..\
@echo off

xcopy "Xaloc\vendor\premake5-imgui.lua" "Xaloc\vendor\imgui\premake5.lua" /Y

call vendor\bin\premake\premake5.exe vs2019
popd
IF %ERRORLEVEL% NEQ 0 (
  PAUSE
)