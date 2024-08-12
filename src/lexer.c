#include "zrx_language.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Token** lexer(const char* input) {
    Token** tokens = malloc(sizeof(Token*) * 1024); // Allocate space for tokens
    int index = 0;
    const char* current = input;

    while (*current) {
        if (isalpha(*current)) { // If it's an identifier
            Token* token = (Token*)malloc(sizeof(Token));
            token->type = TOKEN_IDENTIFIER;
            token->value = strdup(current);
            tokens[index++] = token;
            while (isalpha(*current)) current++; // Move past the identifier
        } else if (*current == '"') { // If it's a string
            current++; // Move past the opening quote
            const char* start = current;
            while (*current && *current != '"') current++;
            Token* token = (Token*)malloc(sizeof(Token));
            token->type = TOKEN_STRING;
            token->value = strndup(start, current - start);
            tokens[index++] = token;
            if (*current) current++; // Move past the closing quote
        } else {
            current++;
        }
    }

    Token* eof_token = (Token*)malloc(sizeof(Token));
    eof_token->type = TOKEN_EOF;
    eof_token->value = NULL;
    tokens[index] = eof_token; // Add EOF token

    return tokens;
}
