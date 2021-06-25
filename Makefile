# Automatisation of compilation ops

COMPILER = g++
BINDIR = ./bin
SRCDIR = ./src
LINKFLAG = -ggdb -Wall -lsqlite3 -lcrypto 
OBJECTFILES = ${BINDIR}/core.o ${BINDIR}/crypt.o ${BINDIR}/auth.o ${BINDIR}/utils.o ${BINDIR}/db.o ${BINDIR}/main.o
DB_PATH = \"${HOME}/.local/share/pass/pass.db\"
DIRNAME = ${HOME}/.local/share/pass/
INIT_SCRIPT = \"${HOME}/.local/share/pass/pass.sql\"
USER_ID := $(shell id -u)

all : pass
.PHONY : all clean install

deny_if_root :
ifeq (${USER_ID}, 0)
	@echo "Please do not run this as root."
	@exit -1
endif

exec : pass
	@${BINDIR}/pass

pass : deny_if_root clean ${OBJECTFILES}
	@if [ ! -d ${DIRNAME} ] ; then mkdir -p ${DIRNAME} ; fi
	cp ./pass.sql ${DIRNAME}
	${COMPILER} ${LINKFLAG} -o ${BINDIR}/pass ${OBJECTFILES}

install : clean pass
	cp ./bin/pass ~/.local/bin/

${BINDIR}/%.o : ${SRCDIR}/%.cpp
	${COMPILER} -ggdb -Wall -D INIT_SCRIPT="${INIT_SCRIPT}" -D DB_PATH="${DB_PATH}" -c -o $@ $^

clean :
	@mkdir -p ${BINDIR}/.tmp
	@rmdir ${BINDIR}/.tmp
	@rm -f ${BINDIR}/*
	@echo "[*] Binary file is removed"
