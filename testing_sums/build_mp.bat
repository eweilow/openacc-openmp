@echo off

SET OUTPUT=".\dist\build_mp.exe"
SET PGCC="pgcc"
SET BUILDCOMMAND=%PGCC% -o=%OUTPUT% -mp=allcores -fast -Minfo=accel -Minfo=mp -Mprof=ccff main.c

mkdir .\dist
echo Removing existing build
del %OUTPUT%

echo Building '%BUILDCOMMAND%'
%BUILDCOMMAND%

echo Starting
%OUTPUT%