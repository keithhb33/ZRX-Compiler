#include "zrx_language.h"
#include <stdlib.h>

void free_command_list(Node* root) {
    Node* current = root;
    while (current) {
        Node* next = current->next;
        free(current->command);
        if (current->arg1) free(current->arg1);
        if (current->arg2) free(current->arg2);
        free(current);
        current = next;
    }
}
