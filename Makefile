cc := gcc

all: build

build:
	cc src/socketServer.c -o server -O3 -Wall
	cc src/socketClient.c -o client -O3 -Wall

