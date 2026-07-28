CC = gcc

LIBS = -lSDL2 -lSDL2_ttf

all:
	$(CC) src/tratty.c src/renderer.c src/screen.c src/core.c $(LIBS) -o build/tratty

run:
	./build/tratty

clean:
	rm -f ./build/tratty
