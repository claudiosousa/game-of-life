GCC=gcc -g -Wall -Wextra -std=gnu99
LIBS=-lSDL2

gfx_example: gfx_example.o gfx.o
	$(GCC) $^ -o $@ $(LIBS)

gfx_example.o: gfx_example.c gfx.h
	$(GCC) $< -c

gfx.o: gfx.c gfx.h
	$(GCC) $< -c

run: gfx_example
	./$<

clean:
	rm -f *.o gfx_example
