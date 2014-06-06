SIZE = 32766
WEVERYTHING = -Waddress -Waggregate-return -Wall -Warray-bounds -Wcast-align -Wcast-qual -Wchar-subscripts -Wclobbered -Wcomment -Wconversion -Wcoverage-mismatch -Wdisabled-optimization -Wempty-body -Wenum-compare -Wextra -Wfloat-equal -Wformat -Wformat-nonliteral -Wformat-security -Wformat-y2k -Wformat=2 -Wframe-larger-than=$(SIZE) -Wignored-qualifiers -Wimplicit -Wimplicit-function-declaration -Wimplicit-int -Winit-self -Winline -Winvalid-pch -Wlarger-than=$(SIZE) -Wlogical-op -Wmain -Wmissing-braces -Wmissing-field-initializers -Wmissing-format-attribute -Wmissing-include-dirs -Wmissing-noreturn -Wno-attributes -Wno-builtin-macro-redefined -Wno-deprecated -Wno-deprecated-declarations -Wno-div-by-zero -Wno-endif-labels -Wno-format-contains-nul -Wno-format-extra-args -Wno-int-to-pointer-cast -Wno-mudflap -Wno-multichar -Wno-overflow -Wno-pointer-to-int-cast -Wno-pragmas -Wnonnull -Woverlength-strings -Wpacked -Wpacked-bitfield-compat -Wparentheses -Wpointer-arith -Wreturn-type -Wsequence-point -Wshadow -Wsign-compare -Wsign-conversion -Wstack-protector -Wstrict-aliasing -Wstrict-overflow=5 -Wswitch -Wswitch-default -Wswitch-enum -Wsync-nand -Wsystem-headers -Wtrigraphs -Wtype-limits -Wundef -Wuninitialized -Wunknown-pragmas -Wunreachable-code -Wunsafe-loop-optimizations -Wunused -Wunused-function -Wunused-label -Wunused-parameter -Wunused-value -Wunused-variable -Wvariadic-macros -Wvla -Wvolatile-register-var -Wwrite-strings \
#		-Wlong-long -Wpadded -Wredundant-decls # system headers
DEBUG = -g \
		-O0 \
		$(WEVERYTHING) # -Weverything
RELEASE = -s \
		-O3 \
		-Wall -Wextra
FLAGS = -std=c89 -D__STDC_VERSION__=199409L \
		-D_POSIX_C_SOURCE=200112L -D_XOPEN_SOURCE=600 \
		$(DEBUG) \
		-I . # actually -D__STDC_VERSION__=198912L
CC = /usr/bin/gcc $(FLAGS)
RM = /bin/rm -f

all: example

example: cheat.h cheat-helpers.h example.c
	$(CC) -o example example.c

clean:
	$(RM) example

.PHONY: all clean
