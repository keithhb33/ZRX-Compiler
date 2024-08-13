#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    char current_path[1024] = ".";

    char dest_path_0[1024];
    snprintf(dest_path_0, sizeof(dest_path_0), "%s/%s", current_path, "Test_Folder");
    mkdir(dest_path_0, 0777);
    snprintf(current_path, sizeof(current_path), "%s/%s", current_path, "Test_Folder");
    chdir(current_path);
    char dest_path_1[1024];
    snprintf(dest_path_1, sizeof(dest_path_1), "%s/%s", current_path, "test_file.txt");
    FILE *fp_1 = fopen(dest_path_1, "w");
    if (fp_1 != NULL) fclose(fp_1);
    printf("%s\n", "Created test_file.txt inside Test_Folder.");
    char *last_slash_2 = strrchr(current_path, '/');
    if (last_slash_2 != NULL) *last_slash_2 = '\0';
    char dest_path_3[1024];
    snprintf(dest_path_3, sizeof(dest_path_3), "%s/%s", current_path, "Another_Folder");
    mkdir(dest_path_3, 0777);
    snprintf(current_path, sizeof(current_path), "%s/%s", current_path, "Another_Folder");
    chdir(current_path);
    char dest_path_4[1024];
    snprintf(dest_path_4, sizeof(dest_path_4), "%s/%s", current_path, "another_file.txt");
    FILE *fp_4 = fopen(dest_path_4, "w");
    if (fp_4 != NULL) fclose(fp_4);
    printf("%s\n", "Created another_file.txt inside Another_Folder.");
    char *last_slash_5 = strrchr(current_path, '/');
    if (last_slash_5 != NULL) *last_slash_5 = '\0';
    char dest_path_6[1024];
    snprintf(dest_path_6, sizeof(dest_path_6), "%s/%s", current_path, "Test_Folder");
    rmdir(dest_path_6);
    char dest_path_7[1024];
    snprintf(dest_path_7, sizeof(dest_path_7), "%s/%s", current_path, "Another_Folder");
    rmdir(dest_path_7);
    return 0;
}
