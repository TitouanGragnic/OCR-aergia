# Simple SDL mini Makefile

CC=gcc

CPPFLAGS= `pkg-config --cflags sdl2` -MMD -lm
CFLAGS= -Wall -Wextra -Werror -fsanitize=address -std=c99 -O3 -lm -g
LDFLAGS= -fsanitize=address
LDLIBS= `pkg-config --libs sdl SDL_image` -lm
EXEC = main

SRC = main.c pixel_operations.c resize.c slicing.c init.c
OBJ = ${SRC:.c=.o}
all: ${EXEC}

${EXEC}: main.o pixel_operations.o resize.o slicing.o init.o
	${CC} ${CFLAGS} ${LDLIBS} $^ -o $@


clean:
	${RM} ${OBJ}${SRC:.c=.d}
	${RM} ${EXEC}
	rm -f output/*.png

# END
