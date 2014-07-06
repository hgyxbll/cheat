@echo off

for %%i in (tests\*) do (
	set n=%%~ni
	set x=%%~xi
	set s=%%~ni.obj
	set o=%%~ni.exe
	set i=%%i
	call :do
)
goto :done
:do
echo --- %i% ---
cl /D__BASE_FILE__="""tests\%i%""" /Fe"tests\%o%" /I. /nologo /w "tests\%i%"
if not exist "tests\%o%" goto :break
"tests\%o%" %*
del /Q "%s%"
:continue
if not exist "tests\%o%" goto :break
del /Q "tests\%o%" 1> nul 2> nul
goto :continue
:break
goto :eof
:done
