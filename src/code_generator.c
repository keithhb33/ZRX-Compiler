#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>
#include "zrx_language.h"

// Function to generate the C code
void generate_code(Node* root, const char* output_file) {
    FILE* file = fopen(output_file, "w");
    if (!file) {
        printf("Error opening output file: %s\n", output_file);
        return;
    }

    // Write necessary includes
    fprintf(file, "#include <stdio.h>\n");
    fprintf(file, "#include <stdlib.h>\n");
    fprintf(file, "#include <string.h>\n");
    fprintf(file, "#include <sys/stat.h>\n");
    fprintf(file, "#include <errno.h>\n");
    fprintf(file, "#include <unistd.h>\n");

    // Start the main function
    fprintf(file, "int main() {\n");

    Node* current = root;
    while (current != NULL) {
        if (strcmp(current->command, "MKDIR") == 0) {
            char* path = current->arg1;
            if (path[0] != '/' && strstr(path, "C:/") != path && strstr(path, "c:/") != path) {
                // Relative path: create the directory in the current working directory
                fprintf(file, "    char cwd[1024];\n");
                fprintf(file, "    if (getcwd(cwd, sizeof(cwd)) != NULL) {\n");
                fprintf(file, "        strcat(cwd, \"/%s\");\n", path);
                fprintf(file, "        if (mkdir(cwd, 0777) == 0) {\n");
            } else {
                // Absolute path: create the directory directly
                fprintf(file, "    if (mkdir(\"%s\", 0777) == 0) {\n", path);
            }
            fprintf(file, "        printf(\"Directory created successfully\\n\");\n");
            fprintf(file, "    } else {\n");
            fprintf(file, "        printf(\"Failed to create directory: %%s\\n\", strerror(errno));\n");
            fprintf(file, "    }\n");
            fprintf(file, "    }\n");  // Close the getcwd block
        }
        // Handle other commands here, e.g., WRITE, APPEND, READ, etc.
        current = current->next;
    }

    // End the main function
    fprintf(file, "    return 0;\n");
    fprintf(file, "}\n");

    fclose(file);
}

// Function to generate the recursive remove function if needed
void generate_recursive_remove_function(FILE* file) {
    fprintf(file, "void remove_directory_recursive(const char* path) {\n");
    fprintf(file, "    struct dirent *entry;\n");
    fprintf(file, "    DIR *dir = opendir(path);\n");

    fprintf(file, "    if (dir == NULL) {\n");
    fprintf(file, "        perror(\"Failed to open directory\");\n");
    fprintf(file, "        return;\n");
    fprintf(file, "    }\n");

    fprintf(file, "    char full_path[1024];\n");
    fprintf(file, "    while ((entry = readdir(dir)) != NULL) {\n");
    fprintf(file, "        if (strcmp(entry->d_name, \".\") == 0 || strcmp(entry->d_name, \"..\") == 0) {\n");
    fprintf(file, "            continue;\n");
    fprintf(file, "        }\n");
    fprintf(file, "        snprintf(full_path, sizeof(full_path), \"%%s/%%s\", path, entry->d_name);\n");
    fprintf(file, "        if (entry->d_type == DT_DIR) {\n");
    fprintf(file, "            remove_directory_recursive(full_path);\n");
    fprintf(file, "        } else {\n");
    fprintf(file, "            if (remove(full_path) != 0) {\n");
    fprintf(file, "                perror(\"Failed to remove file\");\n");
    fprintf(file, "            }\n");
    fprintf(file, "        }\n");
    fprintf(file, "    }\n");

    fprintf(file, "    closedir(dir);\n");
    fprintf(file, "    if (rmdir(path) != 0) {\n");
    fprintf(file, "        perror(\"Failed to remove directory\");\n");
    fprintf(file, "    }\n");
    fprintf(file, "}\n");
}
