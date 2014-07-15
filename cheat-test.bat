@echo off

for %%i in (tests\*) do (
	set c=%2
	set e=%1
	set n=%%~ni
	set s=%%~ni.obj
	set o=tests\%%~ni.exe
	set i=%%~fi
	call :do
)
goto :done
:do
echo --- %n% ---
cl /D__BASE_FILE__="""%i%""" /Fe"%o%" /I. /nologo /w %c% "%i%"
if not exist "%o%" goto :break
"%o%" %e%
del /Q "%s%"
:continue
if not exist "%o%" goto :break
del /Q "%o%" 1> nul 2> nul
goto :continue
:break
goto :eof
:done
