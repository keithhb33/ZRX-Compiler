CC = gcc
CFLAGS = -Wall -Wextra -std=c99

SRC = src/lexer.c src/parser.c src/semantic_analyzer.c src/code_generator.c src/main.c src/utility.c
OBJ = $(SRC:.c=.o)

all: zrx_compiler

zrx_compiler: $(OBJ)
	$(CC) $(CFLAGS) -o zrx_compiler $(OBJ)

clean:
	rm -f $(OBJ) zrx_compiler
