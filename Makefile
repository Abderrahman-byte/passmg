# Automatisation of compilation ops

COMPILER = g++
BINDIR = ./bin

all : pass
.PHONY : all clean

pass : main.cpp
	@${COMPILER} -o ${BINDIR}/pass -O3 main.cpp

clean :
	@touch ${BINDIR}/.tmp
	@rm -f ${BINDIR}/*
	@echo "[*] Binary file is removed"
