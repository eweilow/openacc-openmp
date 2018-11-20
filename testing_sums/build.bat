@echo off

SET PGCC="pgcc"
SET BUILDCOMMAND=%PGCC% -o .\dist\build.exe -fast -g main.c

echo Building '%BUILDCOMMAND%'
%BUILDCOMMAND%

echo Starting
.\dist\build.exe