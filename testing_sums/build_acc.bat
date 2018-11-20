@echo off

SET PGCC="pgcc"
SET BUILDCOMMAND=%PGCC% -o .\dist\build_acc.exe -acc -ta=tesla -fast -Minfo=accel -Mprof=ccff -g main.c

echo Building '%BUILDCOMMAND%'
%BUILDCOMMAND%

echo Starting
.\dist\build_acc.exe