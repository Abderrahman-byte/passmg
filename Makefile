# Automatisation of compilation ops

COMPILER = g++
BINDIR = ./bin
SRCDIR = ./src
LINKFLAG = -lsqlite3 -lcrypto
OBJECTFILES = ${BINDIR}/core.o ${BINDIR}/crypt.o ${BINDIR}/auth.o ${BINDIR}/utils.o ${BINDIR}/db.o ${BINDIR}/main.o

all : pass
.PHONY : all clean

exec : pass
	@${BINDIR}/pass

pass : clean ${OBJECTFILES}
	${COMPILER} ${LINKFLAG} -o ${BINDIR}/pass ${OBJECTFILES}

${BINDIR}/%.o : ${SRCDIR}/%.cpp
	${COMPILER} -c -o $@ $^

clean :
	@touch ${BINDIR}/.tmp
	@rm -f ${BINDIR}/*
	@echo "[*] Binary file is removed"
