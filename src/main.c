#include "zrx_language.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("Usage: %s <zrx file>\n", argv[0]);
        return 1;
    }

    // Parse the .zrx file into a command list
    Node* commands = parse_zrx_file(argv[1]);
    if (!commands) {
        return 1;
    }

    // Generate the output C code
    generate_code(commands, "output.c");

    // Clean up
    free_command_list(commands);
    return 0;
}
