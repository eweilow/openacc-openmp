@echo off

mkdir .\dist\

SET PGI_ACC_TIME=1
SET OUTPUT=".\dist\build_acc_multicore.exe"
SET PGCC="pgcc"
SET BUILDCOMMAND=%PGCC% -o=%OUTPUT% -acc -ta=multicore -fast -Minfo=accel -Mprof=ccff main.c

echo Removing existing build
del %OUTPUT%

echo Building '%BUILDCOMMAND%'
%BUILDCOMMAND%

echo Starting
%OUTPUT%
