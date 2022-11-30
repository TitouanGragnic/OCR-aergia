# Simple SDL mini Makefile

CC=gcc

CPPFLAGS= `pkg-config --cflags sdl2` -MMD -lm
CFLAGS= -Wall -Wextra -Werror -fsanitize=address -std=c99 -O3 -lm -g
LDFLAGS= -fsanitize=address
LDLIBS= `pkg-config --libs sdl SDL_image` -lm
EXEC = main

SRC = $(shell find $(SOURCE_DIR) -name "*.c")
OBJ = ${SRC:.c=.o}
all: ${EXEC}

${EXEC}:${OBJ}
	${CC} ${CFLAGS} ${LDLIBS} $^ -o $@


clean:
	${RM} ${OBJ} ${SRC:.c=.d}
	${RM} ${EXEC}
	${RM} output/slot/*.png
	${RM} output/treatment/*.png

# END
