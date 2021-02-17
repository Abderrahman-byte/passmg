# Automatisation of compilation ops

COMPILER = g++
BINDIR = ./bin
SRCDIR = ./src
LINKFLAG = -lsqlite3

all : pass
.PHONY : all clean

exec : pass
	@${BINDIR}/pass

pass : clean ${BINDIR}/auth.o ${BINDIR}/utils.o ${BINDIR}/db.o ${BINDIR}/main.o
	${COMPILER} ${LINKFLAG} -o ${BINDIR}/pass ${BINDIR}/auth.o ${BINDIR}/db.o ${BINDIR}/utils.o ${BINDIR}/main.o

${BINDIR}/auth.o : ${SRCDIR}/auth.cpp
	${COMPILER} -c -o ${BINDIR}/auth.o ${SRCDIR}/auth.cpp

${BINDIR}/utils.o : ${SRCDIR}/utils.cpp
	${COMPILER} -c -o ${BINDIR}/utils.o ${SRCDIR}/utils.cpp

${BINDIR}/db.o : ${SRCDIR}/db.cpp
	${COMPILER} -c -o ${BINDIR}/db.o ${SRCDIR}/db.cpp

${BINDIR}/main.o : ${SRCDIR}/main.cpp
	${COMPILER} -c -o ${BINDIR}/main.o ${SRCDIR}/main.cpp

clean :
	@touch ${BINDIR}/.tmp
	@rm -f ${BINDIR}/*
	@echo "[*] Binary file is removed"
