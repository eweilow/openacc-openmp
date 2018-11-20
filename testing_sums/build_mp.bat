@echo off

SET PGCC="pgcc"
SET BUILDCOMMAND=%PGCC% -o .\dist\build_mp.exe -mp=allcores -fast -Minfo=accel -Minfo=mp -Mprof=ccff -g main.c

echo Building '%BUILDCOMMAND%'
%BUILDCOMMAND%

echo Starting
.\dist\build_mp.exe