# Automatisation of compilation ops

COMPILER = g++
BINDIR = ./bin
SRCDIR = ./src

all : pass
.PHONY : all clean

pass : clean ${BINDIR}/utils.o ${BINDIR}/main.o
	${COMPILER} -o ${BINDIR}/pass ${BINDIR}/utils.o ${BINDIR}/main.o

${BINDIR}/utils.o : ${SRCDIR}/utils.cpp
	${COMPILER} -c -o ${BINDIR}/utils.o ${SRCDIR}/utils.cpp

${BINDIR}/main.o : ${SRCDIR}/main.cpp
	${COMPILER} -c -o ${BINDIR}/main.o ${SRCDIR}/main.cpp

clean :
	@touch ${BINDIR}/.tmp
	@rm -f ${BINDIR}/*
	@echo "[*] Binary file is removed"
