set cc=cl
set warnings=/W4 ^
	/wd4127
set debug=%warnings% /Od /Zi
set deploy=/Ox
set cflags=%debug% ^
	/D__STDC_VERSION__=199409L ^
	/I.

%cc% %cflags% /D__BASE_FILE__=\"cheat-example.c\" /Fecheat-example cheat-example.c
