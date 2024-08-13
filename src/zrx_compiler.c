#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

void compile_zrx_to_c(const char *input_file, const char *output_file) {
    FILE *input = fopen(input_file, "r");
    FILE *output = fopen(output_file, "w");

    if (input == NULL) {
        perror("Error opening input file");
        exit(EXIT_FAILURE);
    }

    if (output == NULL) {
        perror("Error opening output file");
        exit(EXIT_FAILURE);
    }

    // Write standard includes to the output file
    fprintf(output, "#include <sys/stat.h>\n");
    fprintf(output, "#include <sys/types.h>\n");
    fprintf(output, "#include <unistd.h>\n");
    fprintf(output, "#include <stdio.h>\n");
    fprintf(output, "#include <stdlib.h>\n");
    fprintf(output, "#include <string.h>\n\n");
    fprintf(output, "int main() {\n");
    fprintf(output, "    char current_path[1024] = \".\";\n\n");

    char line[256];
    int unique_id = 0;  // Unique identifier for variables

    while (fgets(line, sizeof(line), input)) {
        char folder_name[256];
        char file_name[256];
        int matched = 0;

        if (strncmp(line, "MKDIR", 5) == 0) {
            // Try parsing with quotes
            if (sscanf(line, "MKDIR \"%[^\"]\"", folder_name) == 1) {
                matched = 1;
            }
            // If not quoted, try without quotes
            else if (sscanf(line, "MKDIR %s", folder_name) == 1) {
                matched = 1;
            }
            if (matched) {
                fprintf(output, "    char dest_path_%d[1024];\n", unique_id);
                fprintf(output, "    snprintf(dest_path_%d, sizeof(dest_path_%d), \"%%s/%%s\", current_path, \"%s\");\n", unique_id, unique_id, folder_name);
                fprintf(output, "    mkdir(dest_path_%d, 0777);\n", unique_id);
                unique_id++;
            }
        } else if (strncmp(line, "RMDIR", 5) == 0) {
            // Try parsing with quotes
            if (sscanf(line, "RMDIR \"%[^\"]\"", folder_name) == 1) {
                matched = 1;
            }
            // If not quoted, try without quotes
            else if (sscanf(line, "RMDIR %s", folder_name) == 1) {
                matched = 1;
            }
            if (matched) {
                fprintf(output, "    char dest_path_%d[1024];\n", unique_id);
                fprintf(output, "    snprintf(dest_path_%d, sizeof(dest_path_%d), \"%%s/%%s\", current_path, \"%s\");\n", unique_id, unique_id, folder_name);
                fprintf(output, "    rmdir(dest_path_%d);\n", unique_id);
                unique_id++;
            }
        } else if (strncmp(line, "TOUCH", 5) == 0 || strncmp(line, "MAKE", 4) == 0) {
            // Try parsing with quotes
            if (sscanf(line, "%*s \"%[^\"]\"", file_name) == 1) {
                matched = 1;
            }
            // If not quoted, try without quotes
            else if (sscanf(line, "%*s %s", file_name) == 1) {
                matched = 1;
            }
            if (matched) {
                fprintf(output, "    char dest_path_%d[1024];\n", unique_id);
                fprintf(output, "    snprintf(dest_path_%d, sizeof(dest_path_%d), \"%%s/%%s\", current_path, \"%s\");\n", unique_id, unique_id, file_name);
                fprintf(output, "    FILE *fp_%d = fopen(dest_path_%d, \"w\");\n", unique_id, unique_id);
                fprintf(output, "    if (fp_%d != NULL) fclose(fp_%d);\n", unique_id, unique_id);
                unique_id++;
            }
        } else if (strncmp(line, "RM", 2) == 0) {
            // Try parsing with quotes
            if (sscanf(line, "RM \"%[^\"]\"", file_name) == 1) {
                matched = 1;
            }
            // If not quoted, try without quotes
            else if (sscanf(line, "RM %s", file_name) == 1) {
                matched = 1;
            }
            if (matched) {
                fprintf(output, "    char dest_path_%d[1024];\n", unique_id);
                fprintf(output, "    snprintf(dest_path_%d, sizeof(dest_path_%d), \"%%s/%%s\", current_path, \"%s\");\n", unique_id, unique_id, file_name);
                fprintf(output, "    remove(dest_path_%d);\n", unique_id);
                unique_id++;
            }
        } else if (strncmp(line, "MOVE", 4) == 0) {
            char source[256], destination[256];
            // Try parsing with quotes
            if (sscanf(line, "MOVE \"%[^\"]\" \"%[^\"]\"", source, destination) == 2) {
                matched = 1;
            }
            // If not quoted, try without quotes
            else if (sscanf(line, "MOVE %s %s", source, destination) == 2) {
                matched = 1;
            }
            if (matched) {
                fprintf(output, "    char source_path_%d[1024], dest_path_%d[1024];\n", unique_id, unique_id);
                fprintf(output, "    snprintf(source_path_%d, sizeof(source_path_%d), \"%%s/%%s\", current_path, \"%s\");\n", unique_id, unique_id, source);
                fprintf(output, "    snprintf(dest_path_%d, sizeof(dest_path_%d), \"%%s/%%s\", current_path, \"%s\");\n", unique_id, unique_id, destination);
                fprintf(output, "    rename(source_path_%d, dest_path_%d);\n", unique_id, unique_id);
                unique_id++;
            }
        } else if (strncmp(line, "COPY", 4) == 0) {
            char source[256], destination[256];
            // Try parsing with quotes
            if (sscanf(line, "COPY \"%[^\"]\" \"%[^\"]\"", source, destination) == 2) {
                matched = 1;
            }
            // If not quoted, try without quotes
            else if (sscanf(line, "COPY %s %s", source, destination) == 2) {
                matched = 1;
            }
            if (matched) {
                fprintf(output, "    char source_path_%d[1024], dest_path_%d[1024];\n", unique_id, unique_id);
                fprintf(output, "    snprintf(source_path_%d, sizeof(source_path_%d), \"%%s/%%s\", current_path, \"%s\");\n", unique_id, unique_id, source);
                fprintf(output, "    snprintf(dest_path_%d, sizeof(dest_path_%d), \"%%s/%%s\", current_path, \"%s\");\n", unique_id, unique_id, destination);
                fprintf(output, "    char command_%d[2048];\n", unique_id);
                fprintf(output, "    snprintf(command_%d, sizeof(command_%d), \"cp -r %%s %%s\", source_path_%d, dest_path_%d);\n", unique_id, unique_id, unique_id, unique_id);
                fprintf(output, "    system(command_%d);\n", unique_id);
                unique_id++;
            }
        } else if (strncmp(line, "ECHO", 4) == 0) {
            char text[256];
            // Try parsing with quotes
            if (sscanf(line, "ECHO \"%[^\"]\"", text) == 1) {
                matched = 1;
            }
            // If not quoted, try without quotes
            else if (sscanf(line, "ECHO %s", text) == 1) {
                matched = 1;
            }
            if (matched) {
                fprintf(output, "    printf(\"%%s\\n\", \"%s\");\n", text);
            }
        } else if (strncmp(line, "CD ..", 5) == 0 || strncmp(line, "CD..", 4) == 0) {
            fprintf(output, "    char *last_slash_%d = strrchr(current_path, '/');\n", unique_id);
            fprintf(output, "    if (last_slash_%d != NULL) *last_slash_%d = '\\0';\n", unique_id, unique_id);
            unique_id++;
        } else if (strncmp(line, "CD", 2) == 0) {
            char dir_name[256];
            // Try parsing with quotes
            if (sscanf(line, "CD \"%[^\"]\"", dir_name) == 1) {
                matched = 1;
            }
            // If not quoted, try without quotes
            else if (sscanf(line, "CD %s", dir_name) == 1) {
                matched = 1;
            }
            if (matched) {
                // Update the current_path variable with the new directory
                fprintf(output, "    snprintf(current_path, sizeof(current_path), \"%%s/%%s\", current_path, \"%s\");\n", dir_name);
                // Change the directory
                fprintf(output, "    chdir(current_path);\n");
            }
        }
    }

    fprintf(output, "    return 0;\n");
    fprintf(output, "}\n");

    fclose(input);
    fclose(output);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <input_file.zrx>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    compile_zrx_to_c(argv[1], "output.c");
    printf("Compilation complete. Output written to output.c\n");
    return 0;
}
