# mini Makefile

CC=gcc

CPPFLAGS= `pkg-config --cflags sdl2` -MMD -lm
CFLAGS= -Wall -Wextra -fsanitize=address -g -std=c99 -O0
LDFLAGS= -fsanitize=address
LDLIBS= `pkg-config --libs sdl SDL_image` -lSDL_image -lSDL2_ttf -lm

SRC = $(shell find $(SOURCE_DIR) -name "*.c" | grep -v "xor.c") ../grid_processing/src/utils/init.c
OBJ = ${SRC:.c=.o}
all: xor digits

xor: training/training.o layer/layer.o neural_network/neural_network.o ../grid_processing/src/utils/init.o utils/img_to_matrix.o utils/pixel_operations.o
xor.o: training/training.h layer/layer.h neural_network/neural_network.h ../grid_processing/include/utils/init.h utils/img_to_matrix.h utils/pixel_operations.h

digits: ${OBJ}
	${CC} ${CFLAGS} ${LDLIBS} $^ -o digits

clean:
	${RM} ${OBJ} ${SRC:.c=.d}
	${RM} *.o
	${RM} *.d
	${RM} xor digits


# END
