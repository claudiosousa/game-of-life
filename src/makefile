FLAGS=-g -g -Wall -Wextra -std=gnu99
LIBS=-lpthread -lSDL2

gameoflife: main.c keyboard.o display.o
	cd ./vendor/ && make
	gcc $(FLAGS) -o $@ vendor/gfx.o vendor/keyboard_utils.o $^ $(LIBS)

keyboard.o: keyboard.c keyboard.h
	gcc $(FLAGS) $< -c

display.o: display.c display.h
	gcc $(FLAGS) $< -c

run: gameoflife
	./$< 240 135 0 0.75 30 8

clean:
	rm -f *.o gameoflife
	cd ./vendor/ && make clean
