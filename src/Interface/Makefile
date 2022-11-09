PROG = Aergia

CC = gcc
CFLAGS = `pkg-config --cflags gtk+-3.0` -fsanitize=address -g
LIBS = `pkg-config --libs gtk+-3.0`

aergia: Aergia.c
	${CC} ${CFLAGS} -o ${PROG} ${PROG}.c ${LIBS}

clean:
	rm ${PROG}
