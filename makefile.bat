@echo off

set cc=cl
set warnings=/W4 ^
	/wd4127 /wd4701 /wd4996
set debug=%warnings% /Od /Zi
set deploy=/Ox
set cflags=%debug% ^
	/D__STDC_VERSION__=199409L ^
	/I.

if "%1" == "" goto :return
:continue
call :%1
shift
if "%1" == "" goto break
goto continue
:break
goto :eof
:return

:build
%cc% /nologo %cflags% ^
	/D__BASE_FILE__="""cheat-example.c""" /Fecheat-example.exe cheat-example.c
goto :eof

:clean
del cheat-example.exe
goto :eof

:test
cheat-example.exe -s
cheat-example.exe -d
goto :eof
