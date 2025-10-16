BUILD    = .build

# === CFLAGS and LDFLAGS -------------------------------------------------------
#
CFLAGS  += -std=c11
CFLAGS  += -I.

LDFLAGS += -lm

# === MODS ---------------------------------------------------------------------
#
MODS    += ${BUILD}/weights.o
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

${BUILD}:
	@mkdir -p ${BUILD}

test: compile

fmt:
	~/Workspace/y/tools/clang_format_all.sh .

clean:
	rm -rf ${BUILD}
