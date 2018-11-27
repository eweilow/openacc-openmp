@echo off

SET OUTPUT=".\dist\build_acc_cpu.exe"
SET PGCC="pgcc"
SET BUILDCOMMAND=%PGCC% -o=%OUTPUT% -acc -ta=multicore -fast -Minfo=accel -Mprof=ccff main.c

mkdir .\dist
echo Removing existing build
del %OUTPUT%

echo Building '%BUILDCOMMAND%'
%BUILDCOMMAND%

echo Starting
%OUTPUT%