FLAGS=-g -Wall -Wextra -std=gnu99
LIBS=-lpthread -lSDL2

gameoflife: main.c keyboard.o display.o time_wait.o gfx.o keyboard_utils.o gol_grid.o
	gcc $(FLAGS) -o $@ $^ $(LIBS)

keyboard.o: keyboard.c keyboard.h
	gcc $(FLAGS) $< -c

display.o: display.c display.h
	gcc $(FLAGS) $< -c

gol_grid.o: gol_grid.c gol_grid.h
	gcc $(FLAGS) $< -c

time_wait.o: time_wait.c time_wait.h
	gcc $(FLAGS) $< -c

gfx.o: vendor/gfx.c vendor/gfx.h
	gcc $(FLAGS) $< -c

keyboard_utils.o: vendor/keyboard_utils.c vendor/keyboard_utils.h
	gcc $(FLAGS) $< -c

run: gameoflife
	./$< 240 135 0 0.75 30 8

clean:
	rm -f *.o gameoflife
