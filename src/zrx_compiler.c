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
            int num_args;

            // Try parsing with quotes
            num_args = sscanf(line, "MOVE \"%[^\"]\" \"%[^\"]\"", source, destination);
            if (num_args < 1) {
                // If not quoted, try without quotes
                num_args = sscanf(line, "MOVE %s %s", source, destination);
            }

            if (num_args >= 1) {
                fprintf(output, "    char source_path_%d[1024], dest_path_%d[1024];\n", unique_id, unique_id);
                fprintf(output, "    snprintf(source_path_%d, sizeof(source_path_%d), \"%%s/%%s\", current_path, \"%s\");\n", unique_id, unique_id, source);

                // Extract the destination directory and filename
                char *last_slash = strrchr(destination, '/');
                if (last_slash != NULL) {
                    // Separate directory and filename
                    *last_slash = '\0';
                    const char *dir = destination;
                    const char *file = last_slash + 1;

                    // Create the directory if it doesn't exist
                    fprintf(output, "    struct stat st_%d;\n", unique_id);
                    fprintf(output, "    snprintf(dest_path_%d, sizeof(dest_path_%d), \"%%s/%%s\", current_path, \"%s\");\n", unique_id, unique_id, dir);
                    fprintf(output, "    if (stat(dest_path_%d, &st_%d) != 0) {\n", unique_id, unique_id);
                    fprintf(output, "        mkdir(dest_path_%d, 0777);\n", unique_id);
                    fprintf(output, "    }\n");

                    // Move and rename the file
                    fprintf(output, "    snprintf(dest_path_%d, sizeof(dest_path_%d), \"%%s/%%s/%%s\", current_path, \"%s\", \"%s\");\n", unique_id, unique_id, dir, file);
                } else {
                    // Destination is a directory or current path
                    fprintf(output, "    struct stat st_%d;\n", unique_id);
                    fprintf(output, "    snprintf(dest_path_%d, sizeof(dest_path_%d), \"%%s/%%s\", current_path, \"%s\");\n", unique_id, unique_id, destination);
                    fprintf(output, "    if (stat(dest_path_%d, &st_%d) != 0) {\n", unique_id, unique_id);
                    fprintf(output, "        mkdir(dest_path_%d, 0777);\n", unique_id);
                    fprintf(output, "        snprintf(dest_path_%d, sizeof(dest_path_%d), \"%%s/%%s/%%s\", current_path, \"%s\", \"%s\");\n", unique_id, unique_id, destination, strrchr(source, '/') ? strrchr(source, '/') + 1 : source);
                    fprintf(output, "    } else {");
                    fprintf(output, "        snprintf(dest_path_%d, sizeof(dest_path_%d), \"%%s/%%s\", current_path, \"%s\");\n", unique_id, unique_id, destination);
                    fprintf(output, "    }\n");
                }

                // Move the file
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

    // Generate the base name without the .zrx extension
    char base_name[1024];
    strncpy(base_name, argv[1], sizeof(base_name) - 1);
    base_name[sizeof(base_name) - 1] = '\0';
    char *ext = strrchr(base_name, '.');
    if (ext != NULL) {
        *ext = '\0';  // Remove the extension
    }

    // Generate the names for the .c and executable files based on the input .zrx file
    char output_c[1024];
    char output_executable[1024];

    snprintf(output_c, sizeof(output_c), "%s.c", base_name);
    snprintf(output_executable, sizeof(output_executable), "%s", base_name);

    compile_zrx_to_c(argv[1], output_c);
    printf("Compilation complete. Output written to %s\n", output_c);

    // Compile the output.c file into an executable
    char compile_command[2048];
    snprintf(compile_command, sizeof(compile_command), "gcc %s -o %s", output_c, output_executable);
    int result = system(compile_command);
    if (result != 0) {
        fprintf(stderr, "Error during compilation. Exiting.\n");
        exit(EXIT_FAILURE);
    }
    printf("Compiled %s into executable '%s'\n", output_c, output_executable);

    // Delete the output.c file
    remove(output_c);

    // Run the compiled executable
    char run_command[1024];
    snprintf(run_command, sizeof(run_command), "./%s", output_executable);
    system(run_command);

    return 0;
}
