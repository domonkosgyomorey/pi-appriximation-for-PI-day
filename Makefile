all: build run

main: main.c
	gcc $< -lSDL2 -lSDL2main -lm -O3 -o $@

build: main

run: main
	./main