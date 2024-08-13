# ZRX Compiler

This is a simple compiler for `.zrx` files that converts them into C code. The compiler supports several file management commands, including creating directories, creating and removing files, copying, moving, and removing directories.

## Supported Commands

- **MKDIR "Directory_Name"**: Creates a new directory with the specified name.

- **RMDIR "Directory_Name"**: Removes the specified directory.

- **TOUCH "File_Name"** or **MAKE "File_Name"**: Creates an empty file with the specified name. Both `TOUCH` and `MAKE` can be used interchangeably.

- **RM "File_Name"**: Removes the specified file.

- **MOVE "Source" "Destination"**: Moves or renames a file or directory from the source path to the destination path.

- **COPY "Source" "Destination"**: Copies a file or directory from the source path to the destination path.

- **ECHO "Text"**: Prints the specified text to the console.

## Project Structure

- `input.zrx`: The input file containing `.zrx` commands.
- `src/zrx_compiler.c`: The source code of the compiler.
- `output.c`: The generated C file (after running the compiler).
- `Makefile`: Script for building and running the project.

## Usage

1. **Compile and Run:**

   ```bash
   ./zrx src/input.zrx

## Manual Steps

1. **Build the Compiler:**

   ```bash
   make build

2. **Run the Compiler:**

   ```bash
   make run

3. **Compile the Generated C Code:**

   ```bash
   gcc output.c -o output

4. **Run the Generated Program:**

   ```bash
   ./output