@echo off

SET OUTPUT=".\dist\build_acc.exe"
SET PGCC="pgcc"
SET BUILDCOMMAND=%PGCC% -o=%OUTPUT% -fast main.c

echo Removing existing build
del %OUTPUT%

echo Building '%BUILDCOMMAND%'
%BUILDCOMMAND%

echo Starting
%OUTPUT%