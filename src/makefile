FLAGS=-g -g -Wall -Wextra -std=gnu99
LIBS=-lSDL2

gameoflife: main.c
	cd ./vendor/ && make
	gcc $(FLAGS) -o $@ vendor/gfx.o vendor/keyboard_utils.o $^ $(LIBS)

run: gameoflife
	./$< 240 135 0 0.75 30 8

clean:
	rm -f *.o gameoflife
	cd ./vendor/ && make clean
