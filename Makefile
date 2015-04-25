
SHELL=/bin/sh
MAKEFLAGS=-j20

# Look for every C/C++ file and save their names into .objs/file.o
SRC = $(notdir $(wildcard cb/*.cpp))      \
       $(notdir $(wildcard h9/*.c))       \
	   $(notdir $(wildcard scrypt/*.cpp)) \
	   $(wildcard *.cpp)
OBJS = $(addprefix .objs/, $(patsubst %.c, %.o, $(patsubst %.cpp, %.o, ${SRC})))

CPLUS = g++

INC =                           \
        -I.                     \
        -DNDEBUG                \
        -DTESTNET               \
        -DWANT_DENSE            \

#-DCLAM                  \
#-DBITCOIN               \
#-DDARKCOIN              \
#-DJUMBUCKS              \
#-DLITECOIN              \
#-DPEERCOIN              \
#-DFEDORACOIN            \
#-DMYRIADCOIN            \
#-DUNOBTANIUM            \
#-DPROTOSHARES           \

COPT =                          \
        -m64                    \
        -Wall                   \
        -flto                   \
        -msse3                  \
        -Wextra                 \
        -Wformat                \
        -pedantic               \
        -std=c++0x              \
        -ffast-math             \
        -march=native           \
        -fno-check-new          \
        -funroll-loops          \
        -Wno-deprecated         \
        -fstrict-aliasing       \
        -Wformat-security       \
        -Wstrict-aliasing=2     \
        -Wno-variadic-macros    \
        -fomit-frame-pointer    \
        -Wno-unused-variable    \
        -Wno-unused-parameter   \


LIBS =                          \
    -lcrypto                    \
    -ldl                        \

all:COPT += -g0 -O6
all:LOPT = -s
all:parser

debug:COPT += -g -O0
debug:parser

.objs/%.o : %.cpp
	@echo c++ -- $<
	@mkdir -p .deps
	@mkdir -p .objs
	@${CPLUS} -MD ${INC} ${COPT}  -c $< -o .objs/$*.o
	@mv .objs/$*.d .deps

.objs/%.o : cb/%.cpp
	@echo c++ -- $<
	@mkdir -p .deps
	@mkdir -p .objs
	@${CPLUS} -MD ${INC} ${COPT}  -c $< -o .objs/$*.o
	@mv .objs/$*.d .deps

.objs/%.o : h9/%.c
	@echo 'cc ' -- $<
	@mkdir -p .deps
	@mkdir -p .objs
	@${CC} -MD ${INC} ${COPT}  -w -c $< -o .objs/$*.o
	@mv .objs/$*.d .deps

.objs/%.o : scrypt/%.cpp
	@echo c++ -- $<
	@mkdir -p .deps
	@mkdir -p .objs
	@${CPLUS} -MD ${INC} ${COPT}  -c $< -o .objs/$*.o
	@mv .objs/$*.d .deps

parser:${OBJS}
	@echo lnk -- parser 
	@${CPLUS} ${LOPT} ${COPT} -o parser ${OBJS} ${LIBS}

clean:
	-rm -rf *.i .objs .deps *.d parser

# Usage: make print-VARIABLE to debug this Makefile
print-%  : ; @echo $* = $($*)

-include .deps/*
