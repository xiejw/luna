UNAME     = $(shell uname)
BUILD     = .build
SHELL     = bash

# === CFLAGS and LDFLAGS -------------------------------------------------------
#
CFLAGS  += -std=c11 -Wall -Werror -pedantic -Wextra -Wfatal-errors -Wconversion
CFLAGS  += -Wno-unused-parameter
CFLAGS  += -I.

LDFLAGS += -lm

ifdef RELEASE
CFLAGS   += -DNDEBUG -O3 -march=native
LDFLAGS  += -ffast-math
else
CFLAGS   += -g
endif

ifdef ASAN
CFLAGS   += -fsanitize=address
endif

# === MODS ---------------------------------------------------------------------
#
MODS    += ${BUILD}/qwen3.o
MODS    += ${BUILD}/ops.o
MODS    += ${BUILD}/sds.o
MODS    += ${BUILD}/vec.o
MODS    += ${BUILD}/main.o

# === Rules --------------------------------------------------------------------
#
.PHONY: run

run: ${BUILD}/a.out
	@$<

${BUILD}/a.out: ${MODS} | ${BUILD}
	${CC} -o $@ ${LDFLAGS} ${MODS}

${BUILD}/%.o: src/%.c | ${BUILD}
	${CC} ${CFLAGS} -o $(shell printf "%-20s" $@) -c $<

${BUILD}/%.o: cmd/%.c | ${BUILD}
	${CC} ${CFLAGS} -o $(shell printf "%-20s" $@) -c $<

compile: ${MODS}

# === House Keeping ------------------------------------------------------------
#
${BUILD}:
	@mkdir -p ${BUILD}

test: compile

fmt:
	~/Workspace/y/tools/clang_format_all.sh .

clean:
	rm -rf ${BUILD}
