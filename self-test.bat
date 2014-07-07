@echo off

for %%i in (tests\*) do (
	set n=%%~ni
	set s=%%~ni.obj
	set o=tests\%%~ni.exe
	set i=%%~fi
	call :do
)
goto :done
:do
echo --- %n% ---
cl /D__BASE_FILE__="""%i%""" /Fe"%o%" /I. /nologo /w "%i%"
if not exist "%o%" goto :break
"%o%" %*
del /Q "%s%"
:continue
if not exist "%o%" goto :break
del /Q "%o%" 1> nul 2> nul
goto :continue
:break
goto :eof
:done
