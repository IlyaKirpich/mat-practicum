#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int ten_to_four(char symbol){
    if (symbol >= 'A' && symbol <= 'Z'){
        symbol = tolower(symbol);
    }
    int asci = symbol;
    int result = 0;
    int multiplier = 1;
    while (asci != 0){
        result += asci % 4 * multiplier;
        multiplier *= 10;
        asci /= 4;
    }
    return result;
}

int ten_to_eight(char symbol){
    int asci = symbol;
    int result = 0;
    int multiplier = 1;
    while (asci != 0){
        result += asci % 8 * multiplier;
        multiplier *= 10;
        asci /= 8;
    }
    return result;
}

void r_function (FILE *file1, FILE *file2, FILE *file3){
    char a1, a2;
    a1 = fgetc(file1);
    a2 = fgetc(file2);
    while (a1 != EOF || a2 != EOF){
        if (a1 != EOF){
            while(a1 != EOF && a1 != '\n' && a1 !=' '){
                fputc(a1, file3);
                a1 = fgetc(file1);
            }
            while (a1 == '\n' || a1 == ' ')
                a1 = fgetc(file1);
            fputc(' ', file3);
        }
        if (a2 != EOF){
            while(a2 != EOF && a2 != '\n' && a2 !=' '){
                fputc(a2, file3);
                a2 = fgetc(file2);
            }
            while (a2 == '\n' || a2 == ' ')
                a2 = fgetc(file2);
            fputc(' ', file3);
        }
    }
}

void a_function(FILE* file1, FILE* file2){
    int lexeme_number = 1;
    char a = fgetc(file1);
    while (a != EOF){
        if (lexeme_number % 10 == 0){
            while (a != EOF && a != ' ' && a != '\n'){
                int symbol = ten_to_four(a);
                fprintf(file2, "%d", symbol);
                a = fgetc(file1);
            }
            fputc(' ', file2);
        } else if (lexeme_number % 5 == 0){
            while (a != EOF && a != ' ' && a != '\n'){
                int symbol = ten_to_eight(a);
                fprintf(file2, "%d", symbol);
                a = fgetc(file1);
            }
            fputc(' ', file2);
        } else if (lexeme_number % 2 == 0){
            while (a != EOF && a != ' ' && a != '\n'){
                fputc(tolower(a), file2);
                a = fgetc(file1);
            }
            fputc(' ', file2);
        } else {
            while (a != EOF && a != ' ' && a != '\n'){
                fputc(a, file2);
                a = fgetc(file1);
            }
            fputc(' ', file2);
        }
        while (a == ' ' || a == '\n')
            a = fgetc(file1);
        lexeme_number++;
    }
}

int flag_validation(char* flag){
    if (flag[2] != 0) return 0;
    if (flag[0] != '-' && flag[0] != '/' || flag[1] != 'r' && flag[1] != 'a') return 0;
    return 1;
}

int arguments_amount_validation(int argc, char flag){
    if (flag == 'a' && argc != 4) return 0;
    if (flag == 'r' && argc != 5) return 0;
    return 1;
}

int main(int argc, char* argv[]){
    if (!flag_validation(argv[1])){
        printf("Wrong flag detected");
        return 1;
    }
    char flag = argv[1][1];
    if (!arguments_amount_validation(argc, flag)) return 2;
    FILE *file1, *file2, *file3;
    switch(flag){
        case 'r':
            file1 = fopen(argv[2], "r");
            if (file1 == NULL){
                printf("Wrong name of the first file");
                return 3;
            }
            file2 = fopen(argv[3], "r");
            if (file2 == NULL){
                printf("Wrong name of the second file");
                return 3;
            }
            file3 = fopen(argv[4], "w");
            if (file3 == NULL){
                printf("Wrong name of the third file");
                return 3;
            }
            r_function(file1, file2, file3);
            fclose(file1);
            fclose(file2);
            fclose(file3);
            break;
        case 'a':
            file1 = fopen(argv[2], "r");
            if (file1 == NULL){
                printf("Wrong name of the first file");
                return 3;
            }
            file2 = fopen(argv[3], "w");
            if (file2 == NULL){
                printf("Wrong name of the second file");
                return 3;
            }
            a_function(file1, file2);
            fclose(file1);
            fclose(file2);
            break;
    }
    return 0;
}