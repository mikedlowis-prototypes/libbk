#------------------------------------------------------------------------------
# Build Configuration
#------------------------------------------------------------------------------
# Update these variables according to your requirements.

# tools
CC = c99
LD = ${CC}
AR = ar

# flags
INCS      = -Isource/ -Itests/
CPPFLAGS  = -D_XOPEN_SOURCE=700  -DBK_DEBUG_TRACE
CFLAGS   += ${INCS} ${CPPFLAGS}
LDFLAGS  += ${LIBS}
ARFLAGS   = rcs

#------------------------------------------------------------------------------
# Build Targets and Rules
#------------------------------------------------------------------------------
SRCS = source/alg.c source/drc.c source/main.c
OBJS = ${SRCS:.c=.o}
LIB  = libbk.a
TEST_SRCS = tests/main.c
TEST_OBJS = ${TEST_SRCS:.c=.o}
TEST_BIN  = testbk

all: options ${LIB} ${TEST_BIN}

options:
	@echo "Toolchain Configuration:"
	@echo "  CC       = ${CC}"
	@echo "  CFLAGS   = ${CFLAGS}"
	@echo "  LD       = ${LD}"
	@echo "  LDFLAGS  = ${LDFLAGS}"
	@echo "  AR       = ${AR}"
	@echo "  ARFLAGS  = ${ARFLAGS}"

${LIB}: ${OBJS}
	@echo AR $@
	@${AR} ${ARFLAGS} $@ ${OBJS}

${TEST_BIN}: ${TEST_OBJS} ${LIB}
	@echo LD $@
	@${LD} -o $@ ${TEST_OBJS} ${LIB} ${LDFLAGS}
	-./$@

.c.o:
	@echo CC $<
	@${CC} ${CFLAGS} -c -o $@ $<

clean:
	@rm -f ${LIB} ${TEST_BIN} ${OBJS} ${TEST_OBJS}

.PHONY: all options

