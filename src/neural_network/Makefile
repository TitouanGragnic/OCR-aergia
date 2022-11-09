# mini Makefile

CC=gcc

CPPFLAGS= -MMD
CFLAGS= -Wall -Wextra -fsanitize=address -g -std=c99 -O0
LDFLAGS= -fsanitize=address
LDLIBS= -lm

all: xor

xor : training.o layer.o neural_network.o
xor.o : training.h layer.h neural_network.h

clean:
	${RM} *.o
	${RM} *.d
	${RM} xor


# END
