# Automatisation of compilation ops

COMPILER = g++
BINDIR = ./bin
SRCDIR = ./src
LINKFLAG = -lsqlite3 -lcrypto
OBJECTFILES = ${BINDIR}/core.o ${BINDIR}/crypt.o ${BINDIR}/auth.o ${BINDIR}/utils.o ${BINDIR}/db.o ${BINDIR}/main.o
DB_PATH = \"${HOME}/.local/share/pass/pass.db\"
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
	${COMPILER} ${LINKFLAG} -o ${BINDIR}/pass ${OBJECTFILES}

install : clean pass
	mkdir -p ~/.local/share/pass
	cp ./pass.sql ~/.local/share/pass
	cp ./bin/pass ~/.local/bin/

${BINDIR}/%.o : ${SRCDIR}/%.cpp
	${COMPILER} -D INIT_SCRIPT="${INIT_SCRIPT}" -D DB_PATH="${DB_PATH}" -c -o $@ $^

clean :
	@mkdir -p ${BINDIR}/.tmp
	@rmdir ${BINDIR}/.tmp
	@rm -f ${BINDIR}/*
	@echo "[*] Binary file is removed"
