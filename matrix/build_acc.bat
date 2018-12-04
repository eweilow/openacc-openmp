@echo off

SET PGI_ACC_TIME=1
SET OUTPUT=".\dist\build_acc.exe"
SET PGCC="pgcc"
SET BUILDCOMMAND=%PGCC% -o=%OUTPUT% -fast -O3 -Minfo=accel -Mprof=ccff main.c

echo Removing existing build
del %OUTPUT%

echo Building '%BUILDCOMMAND%'
%BUILDCOMMAND%

echo Starting
%OUTPUT%