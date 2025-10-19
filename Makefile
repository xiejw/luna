UNAME     = $(shell uname)
BUILD     = .build
SHELL     = bash

ZION_PATH = ../y/ann/zion/cc

# === CXXFLAGS and LDFLAGS -------------------------------------------------------
#
CXXFLAGS  += -std=c++23 -Wall -Werror -pedantic -Wextra -Wfatal-errors -Wconversion
CXXFLAGS  += -Wno-unused-parameter
CXXFLAGS  += -I. -I${ZION_PATH}/include

LDFLAGS += -lm

ifdef RELEASE
CXXFLAGS   += -DNDEBUG -O3 -march=native
LDFLAGS  += -ffast-math
else
CXXFLAGS   += -g
endif

ifdef ASAN
CXXFLAGS   += -fsanitize=address
endif

# === MODS ---------------------------------------------------------------------
#
MODS    += ${BUILD}/qwen3.o
MODS    += ${BUILD}/ops.o
MODS    += ${BUILD}/main.o

# === Rules --------------------------------------------------------------------
#
.PHONY: run

run: ${BUILD}/a.out
	@$<

${BUILD}/a.out: ${MODS} | ${BUILD}
	${CXX} -o $@ ${LDFLAGS} ${MODS}

${BUILD}/%.o: src/%.cc | ${BUILD}
	${CXX} ${CXXFLAGS} -o $(shell printf "%-20s" $@) -c $<

${BUILD}/%.o: cmd/%.cc | ${BUILD}
	${CXX} ${CXXFLAGS} -o $(shell printf "%-20s" $@) -c $<

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
