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

- `src/zrx_compiler.c`: The source code of the compiler.
- `testFile.zrx`: The .zrx file

## Usage

1. **Write .zrx file:**

   Example testFile.zrx:
   ```bash
   MKDIR testDir
   CD testDir
   TOUCH main.py
   CD ..
   MOVE testDir/main.py .

2. **Compile & Add to Path:**

   Windows:
   ```bash
   ./install.bat
   ```
   Unix:
   ```bash
   sh install.sh
   ```

3. **Run .zrx file:**

   ```bash
   zrx testFile.zrx
