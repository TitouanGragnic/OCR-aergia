CC = gcc

CPPFLAGS= `pkg-config --cflags sdl2` -MMD -lm
CFLAGS = `pkg-config --cflags gtk+-3.0` -Werror -Wextra -Wall -O3 -fsanitize=address -g
LIBS = `pkg-config --libs gtk+-3.0`
LDFLAGS= -fsanitize=address
LDLIBS= `pkg-config --libs sdl SDL_image` -lm
EXEC = main Aergia

SRC = $(shell find $(SOURCE_DIR) -name "*.c")
OBJ = ${SRC:.c=.o}
all: ${EXEC}

${EXEC}:${OBJ}
	${CC} ${CFLAGS} ${LDLIBS} $^ -o $@

clean:
	${RM} ${OBJ} ${SRC:.c=.d}
	${RM} ${EXEC}
	rm output/slot/*.png
