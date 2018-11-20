@echo off

SET PGCC="G:\Program Files\PGI\win64\18.10\bin\pgcc"
SET BUILDCOMMAND=%PGCC% -o .\dist\build.exe -fast -g main.c

echo Building '%BUILDCOMMAND%'
%BUILDCOMMAND%

echo Starting
.\dist\build.exe