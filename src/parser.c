#include "zrx_language.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to parse a .zrx file into a command list (AST)
Node* parse_zrx_file(const char* filename) {
    // Open the file
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error: Could not open file %s\n", filename);
        return NULL;
    }

    // Determine the file size
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Allocate memory for the file content
    char* file_content = (char*)malloc(file_size + 1);
    if (!file_content) {
        printf("Error: Could not allocate memory for file content\n");
        fclose(file);
        return NULL;
    }

    // Read the file content
    fread(file_content, 1, file_size, file);
    file_content[file_size] = '\0';  // Null-terminate the string

    fclose(file);

    // Tokenize the file content
    Token** tokens = lexer(file_content);

    // Parse the tokens into a command list (AST)
    Node* command_list = parse(tokens);

    // Clean up the allocated memory for file content
    free(file_content);

    // Free the tokens
    for (int i = 0; tokens[i]->type != TOKEN_EOF; i++) {
        free(tokens[i]->value);
        free(tokens[i]);
    }
    free(tokens);

    return command_list;
}

// Function to parse tokens into a command list (AST)
Node* parse(Token** tokens) {
    Node* root = NULL;
    Node* current = NULL;
    int i = 0;

    while (tokens[i]->type != TOKEN_EOF) {
        if (tokens[i]->type == TOKEN_IDENTIFIER) {
            Node* new_node = (Node*)malloc(sizeof(Node));
            new_node->command = strdup(tokens[i]->value);
            new_node->arg1 = NULL;
            new_node->arg2 = NULL;
            new_node->next = NULL;

            if (tokens[i + 1]->type == TOKEN_STRING) {
                new_node->arg1 = strdup(tokens[i + 1]->value);
                if (tokens[i + 2] && tokens[i + 2]->type == TOKEN_STRING) {
                    new_node->arg2 = strdup(tokens[i + 2]->value);
                    i += 2;
                }
                i++;
            }

            if (!root) {
                root = new_node;
            } else {
                current->next = new_node;
            }
            current = new_node;
        }
        i++;
    }

    return root;
}
