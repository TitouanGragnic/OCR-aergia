PROG = Aergia

CC = gcc
CFLAGS = `pkg-config --cflags gtk+-3.0` -Werror -Wextra -Wall -O3 -fsanitize=address -g
LIBS = `pkg-config --libs gtk+-3.0`

aergia: Aergia.c
	${CC} ${CFLAGS} -o ${PROG} ${PROG}.c ${LIBS}

clean:
	${RM} rm ${PROG}
	${RM} output/slot/*.png
	${RM} output/treatment/*.png
