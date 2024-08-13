# ZRX Compiler

This is a simple compiler for `.zrx` files that converts them into C code. The compiler currently supports the `MKDIR` command, which creates directories.

## Project Structure

- `input.zrx`: The input file containing `.zrx` commands.
- `src/zrx_compiler.c`: The source code of the compiler.
- `output.c`: The generated C file (after running the compiler).
- `Makefile`: Script for building and running the project.

## Usage

1. **Build the Compiler:**

   ```bash
   make build
   make run
   gcc -o output.c output
   ./output

