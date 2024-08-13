CC = gcc
CFLAGS = -Wall
SRC = src/zrx_compiler.c
OUT = zrx_compiler

build:
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)

run: build
	./$(OUT)

clean:
	rm -f $(OUT) output.c
