#ifndef ZRX_LANGUAGE_H
#define ZRX_LANGUAGE_H
#include <stdio.h>


typedef struct Token {
    int type;
    char* value;
} Token;

typedef struct Node {
    char* command;
    char* arg1;
    char* arg2;
    struct Node* next;
} Node;

// Token types
#define TOKEN_EOF         0
#define TOKEN_IDENTIFIER  1
#define TOKEN_STRING      2

// Function prototypes
Token** lexer(const char* input);
Node* parse_zrx_file(const char* filename);
void free_command_list(Node* root);
Node* parse(Token** tokens);
void generate_code(Node* root, const char* output_file);
void generate_recursive_remove_function(FILE* file);

#endif // ZRX_LANGUAGE_H
