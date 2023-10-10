#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int new_number(FILE* file1, char* number, char* symbolfirst){
    int number_length = 0;
    char symbol = *symbolfirst;
    if (symbol == EOF) return -2;
    while (symbol == '0'){
        symbol = fgetc(file1);
    }
    while (symbol != EOF && symbol != '\n' && symbol != ' ' && number_length < 30){
        number[number_length] = symbol;
        number_length++;
        symbol = fgetc(file1);
    }
    if (number_length == 30){
        return -1;
    }
    *symbolfirst = symbol;
    return number_length;
}

void skip_delimiter(FILE* file1, char* symbol_1){
    char symbol = *symbol_1;
    while (symbol == ' ' || symbol == '\n'){
        symbol = fgetc(file1);
    }
    *symbol_1 = symbol;
}

int find_base(char* number, int number_length){
    int base_result = 2;
    int base;
    char symbol;
    for(int i = 0; i < number_length; i++){
        symbol = toupper(number[i]);
        if (symbol >= '0' && symbol <= '9'){
            base = symbol - '0';
        }else if(symbol >= 'A' && symbol <= 'Z'){
            base = symbol - 'A' + 10;
        }else return 0;
        if (base_result < base) base_result = base;
    }
    return base_result + 1;
}

int to_decimal(char* number, int base, int number_length){
    int result = 0;
    char symbol;
    for (int i = 0; i < number_length; i++)
    {
        symbol = toupper(number[i]);
        result = result * base + (isdigit(symbol) ? symbol - '0' : symbol - 'A' + 10);
    }
    if (result < 0) return -2;
    return result;
}

int main(int argc, char* argv[]){
    if (argc != 3){
        printf("Wrong amount of arguments");
        return 1;
    }
    FILE* file1 = fopen(argv[1], "r");
    if (!file1){
        printf("No such file or directory");
        return 2;
    }
    FILE* file2 = fopen(argv[2], "w");
    if (!file2){
        printf("No such file or directory");
        return 2;
    }
    char* number = (char*)malloc(sizeof(char) * 30);
    char symbol = fgetc(file1);
    int number_length;
    int base;
    int result;
    while (symbol != EOF){
        skip_delimiter(file1, &symbol);
        number_length = new_number(file1, number, &symbol);
        if (number_length == -2) break;
        if (number_length == -1) fprintf(file2, "integer type overflow\n");
        if (number_length == 0){
            fprintf(file2, "0 2 0\n");
            continue;
        }
        base = find_base(number, number_length);
        result = to_decimal(number, base, number_length);
        if (result == -2) {
            fprintf(file2, "integer type overflow\n");
            continue;
        }
        for(int i = 0; i < number_length; i++){
            fprintf(file2, "%c", number[i]);
        }
        fprintf(file2, " %d %d\n", base, result);
    }

}