CC = gcc

CPPFLAGS= `pkg-config --cflags sdl2` -MMD -lm
CFLAGS = `pkg-config --cflags gtk+-3.0` -Werror -Wextra -Wall -O3 -fsanitize=address -g
LIBS = `pkg-config --libs gtk+-3.0`
LDFLAGS= -fsanitize=address
LDLIBS= `pkg-config --libs sdl SDL_image` -lm
EXEC =  Aergia

SRC = $(shell find $(SOURCE_DIR) -name "grid_processing/*.c")
OBJ = ${SRC:.c=.o}

SRCI = $(shell find $(SOURCE_DIR) -name "Interface/*.c")
OBJI = ${SRC:.c=.o}
all: ${EXEC}

${EXEC}:${OBJI}
	${CC} ${CFLAGS} ${LDLIBS} $^ -o $@

main:	-wholename grid_processing/main.c
	${CC} ${CFLAGS} ${LDLIBS} ${OBJ} $^ -o $@


clean:
	${RM} ${OBJI} ${SRCI:.c=.d}
	${RM} ${OBJ} ${SRC:.c=.d}
	${RM} ${EXEC}
	rm output/slot/*.png
