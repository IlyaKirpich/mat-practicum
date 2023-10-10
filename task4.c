#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void d_function(FILE *file1, FILE *file2){
    char a = fgetc(file1);
    while (a != EOF) {
        if (a < '0' || a > '9')
            fputc(a, file2);
        a = fgetc(file1);
    }
    return;
}

void i_function(FILE *file1, FILE *file2){
    char a = fgetc(file1);
    int letter_count = 0;
    while (a != EOF) {
        while (a != EOF && a != '\n') {
            if (a >= 'A' && a <= 'Z' || a >= 'a' && a <= 'z')
                letter_count++;
            a = fgetc(file1);
        }
        fprintf(file2, "%d\n", letter_count);
        letter_count = 0;
        a = fgetc(file1);
    }
}

void s_function(FILE *file1, FILE *file2){
    char a = fgetc(file1);
    int symbols_count = 0;
    while (a != EOF) {
        while (a != EOF && a != '\n') {
            if (!(a >= 'A' && a <= 'Z' || a >= 'a' && a <= 'z' || a == ' ' || a >= '0' && a <= '9'))
                symbols_count++;
            a = fgetc(file1);
        }
        fprintf(file2, "%d\n", symbols_count);
        symbols_count = 0;
        a = fgetc(file1);
    }
}

void a_function(FILE *file1, FILE *file2){
    char a = fgetc(file1);
    int asci;
    while (a != EOF) {
        if (a < '0' || a > '9')
            fprintf(file2, "%x", a);
        else 
            fputc(a, file2);
        a = fgetc(file1);
    }
}

void new_file(char* filename, char* new_filename, int filename_length){
    filename_length--;
    while (filename[filename_length] != 92 && filename_length != -1){
        new_filename[filename_length+4] = filename[filename_length];
        filename_length--;
    }
    new_filename[filename_length+4] = '_';
    new_filename[filename_length+3] = 't';
    new_filename[filename_length+2] = 'u';
    new_filename[filename_length+1] = 'o';
    while (filename_length != -1){
        new_filename[filename_length] = filename[filename_length];
        filename_length--;
    }
}

int flag_validation(char* flag){
    if (flag[2] != 0 && flag[3] != 0){
        return 0;
    }
    if (flag[2] == 0){
        if (!((flag[0] == '-' || flag[0] == '/') && (flag[1] == 'd' || flag[1] == 'i' || flag[1] == 's' || flag[1] == 'a')))
            return 0;
    }else if (flag[3] == 0)
        if (!((flag[0] == '-' || flag[0] == '/') && flag[1] == 'n' && 
        (flag[2] == 'd' || flag[2] == 'i' || flag[2] == 's' || flag[2] == 'a'))){
            return 0;
        }
    return 1;
}

int main (int argc, char* argv[]){
    if (argc < 3 || argc > 4){
        printf("Wrong amount of arguments");
        return 1;
    }
    if(!flag_validation(argv[1])){
        printf("Wrong flag detected");
        return 2;
    }
    if (argv[1][1] == 'n'){
        if (argc != 4){
        printf("Wrong amount of arguments");
        return 1;
        }
    } else if (argc != 3){
        printf("Wrong amount of arguments");
        return 1;
    }
    FILE *file1 = fopen(argv[2], "r");
    if (file1 == NULL){
        printf ("No such file or directory");
        return 3;
    }
    FILE *file2;
    if (argv[1][1] == 'n'){
        file2 = fopen(argv[3], "w");
        if (file2 == NULL){
            printf ("No such file or directory");
            return 3;
        }
    }else{
        int filename_length = strlen(argv[2]);
        char* filename = (char*)malloc(sizeof(char) * (filename_length+4));
        if (!(filename)){
            printf("Allocation error");
            return 4;
        }
        new_file(argv[2], filename, filename_length);
        printf("%c ", filename[1]);
        file2 = fopen(filename, "w");
        if (file2 == NULL){
            printf ("No such file or directory");
            return 3;
        }
    }
    char flag;
    if (argv[1][1] == 'n') flag = argv[1][2];
    else flag = argv[1][1];
    switch(flag){
        case 'd':
            d_function(file1, file2);
            break;
        case 'i':
            i_function(file1, file2);
            break;
        case 's':
            s_function(file1, file2);
            break;
        case 'a':
            a_function(file1, file2);
            break;
    }
    fclose(file1);
    fclose(file2);
    return 0;
}