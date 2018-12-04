@echo off

SET PGI_ACC_TIME=0
SET OUTPUT=".\dist\build_acc.exe"
SET PGCC="pgcc"
SET BUILDCOMMAND=%PGCC% -o=%OUTPUT% -acc -ta=multicore -O3 -fast main.c

echo Removing existing build
del %OUTPUT%

echo Building '%BUILDCOMMAND%'
%BUILDCOMMAND%

echo Starting
%OUTPUT%