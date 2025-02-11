# Makefile

CFLAGS  := -g -W -Wall -pedantic -fPIC -DPIC
LDFLAGS := -g
LDLIBS  :=

all:	libplugin.so main

clean:
	rm -f *.o *.so 2>/dev/null || true

main.o plugin.o: plugin.h

libplugin.so: LDFLAGS += -shared
libplugin.so: plugin.o
	${CC} -o $@ ${LDFLAGS} $^ ${LIBS}

main: LDLIBS += -ldl
main: main.o
