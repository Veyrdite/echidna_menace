CC=gcc
CFLAGS=-Wall -g
SDL= `sdl-config --cflags --libs` -lSDL_image
FILES=main.c

all:
	${CC} ${CFLAGS} ${FILES} -o echidna_game ${SDL}
