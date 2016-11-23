FLAGS=-g -Wall -Wextra -std=gnu99
LIBS=-lSDL2

gfx_example: gfx_example.o gfx.o keyboard_utils.o
	gcc $(FLAGS) $^ -o $@ $(LIBS)

gfx_example.o: gfx_example.c gfx.h
	gcc $(FLAGS) $< -c

gfx.o: gfx.c gfx.h
	gcc $(FLAGS) $< -c

keyboard_utils.o: keyboard_utils.c keyboard_utils.h
	gcc $(FLAGS) $< -c

run: gfx_example
	./$<

clean:
	rm -f *.o gfx_example
