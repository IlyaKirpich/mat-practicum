#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct{
    char* name;
    int value;
} MemoryCell;

typedef enum{
    ok,
    allocation_error,
    end,
    variable,
    wrong_string,
    wrong_arguments_amount,
    wrong_file
} status;

status new_cell(char* name, MemoryCell** new){
    *new = (MemoryCell*)malloc(sizeof(MemoryCell));
    if (!(*new)) return allocation_error;
    (*new)->name = name;
    (*new)->value = 0;
}

int is_delimiter(char symbol){
    if (symbol == ' ' || symbol == '\t') return 1;
    return 0;
}

int is_operand(char symbol){
    if (symbol == '-' || symbol == '/' || symbol == '*' || symbol == '+' || symbol == '%' || symbol == '='){
        return 1;
    }
    return 0;
}

status scan_string(FILE* file, char** string){
    char symbol = fgetc(file);
    int index = 0;
    int capacity = 2;
    *string = (char*)malloc(sizeof(char)* 2);
    if (!(*string)) return allocation_error;
    while(symbol != '\n' && symbol != EOF){
            (*string)[index] = symbol;
            index++;
            if (index+1 == capacity){
                capacity *= 2;
                char* ptr = realloc(*string, sizeof(char) * capacity);
                if (!ptr) {
                    free(*string);
                    return allocation_error;
                }
                *string = ptr;
        }
        symbol = fgetc(file);
    }
    (*string)[index] = '\0';
    char* ptr = realloc((*string), sizeof(char)* index+1);
    if (!ptr){
        free(*string);
        return allocation_error;
    }
    *string = ptr;
    if (symbol == EOF) return end;
    return ok;
}

int comp(const MemoryCell **first, const MemoryCell ** second){
    return strcmp((*first)->name, (*second)->name);
}

int dichotomy_search(MemoryCell** vector, const int cell_amount, const char* string){
    int current;
    int left = 0;
    int right = cell_amount-1;
    int flag;
    while (left <= right){
        current = (left + right) / 2;
        flag = strcmp(vector[current]->name, string);
        if (flag > 0){
            right = current - 1;
        }
        else if (flag < 0){
            left = current + 1;
        }
        else return current;
    }
    return -1;
}

status new_variable(MemoryCell*** vector, char* variable, int* variables_amount, int* vector_capacity){
    (*variables_amount)++;
    if (*variables_amount == *vector_capacity){
        (*vector_capacity) *= 2;
        MemoryCell** ptr = (MemoryCell**)realloc(*vector, *vector_capacity);
        if (!ptr){
            return allocation_error;
        }
        *vector = ptr;
    }
    MemoryCell* new = NULL;
    if (new_cell(variable, &new) == allocation_error) {
        return allocation_error;
    }
    (*vector)[(*variables_amount) - 1] = new;
    qsort(*vector, *variables_amount, sizeof(MemoryCell*), (int(*) (const void *, const void *))comp);
    return ok;
}

int scan_number(char** ptr){
    int result = 0;
    while (isdigit(**ptr)){
        result = result * 10 + ((**ptr) - '0');
        ++(*ptr);
    }
    return result;
}

status scan_command(char** ptr, char** string){
    int capacity = 2;
    int length = 0;
    *string = (char*)malloc(sizeof(char) * capacity);
    while (isalpha(**ptr)){
        ++length;
        if (length + 1 == capacity){
            capacity *= 2;
            char* ptr2 = (char*)realloc(*string, sizeof(char) * capacity);
            if (!ptr2) return allocation_error;
            *string = ptr2;
        }
        (*string)[length-1] = **ptr;
        ++(*ptr);
    }
    (*string)[length] = '\0';
    return ok;
}

void print_vector(MemoryCell** vector, const int cells_amount){
    for (int i = 0; i < cells_amount; i++){
        printf("%s = %d\n", vector[i]->name, vector[i]->value);
    }
}

int calculation(int number1, int number2, char operator){
    switch(operator){
        case '-':
            return number1 - number2;
        case '+':
            return number1 + number2;
        case '*':
            return number1 * number2;
        case '/':
            return number1 / number2;
        case '%':
            return number1 % number2;
    }
}

status string_processing(char* string0, MemoryCell*** vector, int* cells_amount, int* vector_capacity, const int string_number){
    char* command = NULL;
    int length = strlen(string0);
    char* ptr = string0;
    char* string = NULL;
    if (scan_command(&ptr, &string) == allocation_error){
        return allocation_error;
    }
    if (strcmp(string, "print") == 0){
        free(string);
        string = NULL;
        if (*ptr == ';'){
            print_vector(*vector, *cells_amount);
            return ok;
        }
        else {
            ++ptr;
            scan_command(&ptr, &string);
            int index = dichotomy_search(*vector, *cells_amount, string);
            if (index == -1){
                return wrong_string;
            }
            else {
                printf("%s = %d\n", (*vector)[index]->name, (*vector)[index]->value);
                free(string);
                return ok;
            }
        }
    }
    int index = dichotomy_search(*vector, *cells_amount, string);
    if (index == -1){
        if (new_variable(vector, string, cells_amount, vector_capacity) == allocation_error){
            free(string);
            return allocation_error;
        }
        index = dichotomy_search(*vector, *cells_amount, string);
    }
    string = NULL;
    char* string2 = NULL;
    int number, number2, index2 = -1, index3 = -1;
    ptr++;
    if (isdigit(*ptr)){
        number = scan_number(&ptr);
        if (*ptr == ';'){
            (*vector)[index]->value = number;
            return ok;
        }
    }
    else{
        if (string_number == 0){
            return wrong_string;
        }
        if (scan_command(&ptr, &string) == allocation_error){
            return allocation_error;
        }
        if (*ptr == ';') {
            index2 = dichotomy_search(*vector, *cells_amount, string);
            if (index2 == -1){
                free(string);
                return wrong_string;
            }
            (*vector)[index]->value = (*vector)[index]->value;
            return ok;
        }
        index2 = dichotomy_search(*vector, *cells_amount, string);
    }
    
    char operand = *ptr;
    ++ptr;
    if (isdigit(*ptr)){
        number2 = scan_number(&ptr);
        if ((operand == '/' || operand == '%') && number2 == 0){
            free(string);
            return wrong_string;
        }
        if (index2 == -1){
            (*vector)[index]->value = calculation(number, number2, operand);
        }
        else {
            (*vector)[index]->value = calculation((*vector)[index2]->value, number2, operand);
        }
    }
    else{
        if (string_number == 0){
            return wrong_string;
        }
        if (scan_command(&ptr, &string) == allocation_error){
            return allocation_error;
        }
        index3 = dichotomy_search(*vector, *cells_amount, string);
        if (index3 == -1){
            free(string);
            return wrong_string;
        }
        if ((operand == '/' || operand == '%') && (*vector)[index3]->value == 0){
            free(string);
            return wrong_string;
        }
        if (index2 == -1){
            (*vector)[index]->value = calculation(number, (*vector)[index3]->value, operand);
            return ok;
        }
        (*vector)[index]->value = calculation(number, (*vector)[index3]->value, operand);
        return ok;
    }
    return ok;
}

void delete_vector(MemoryCell*** vector, const int cells_amount, char** string){
    for (int i = 0; i < cells_amount; i++){
        free((*vector)[i]->name);
    }
    free(*vector);
    free(*string);
}

int main(int argc, char* argv[]){
    if (argc != 3){
        printf("Wrong amount of arguments");
        return wrong_arguments_amount;
    }
    FILE* file = fopen(argv[1], "r");
    if (!file){
        printf("File did not open");
        return wrong_file;
    }
    char* string = NULL;
    MemoryCell** vector = (MemoryCell**)malloc(sizeof(MemoryCell*) * 2);
    int cells_amount = 0;
    int capacity = 2;
    status current = scan_string(file, &string);
    status for_command;
    int string_number = 0;
    while (current != allocation_error){
        for_command = string_processing(string, &vector, &cells_amount, &capacity, string_number);
        switch (for_command){
            case allocation_error:
                printf("Allocation error");
                delete_vector(&vector, cells_amount, &string);
                return allocation_error;
            case wrong_string:
                printf("Undeclared cell detected");
                delete_vector(&vector, cells_amount, &string);
                return wrong_string;
            case ok:
                break;
        }
        if (current == end){
            break;
        }
        free(string);
        string = NULL;
        current = scan_string(file, &string);
        string_number++;
    }
    switch(current){
        case allocation_error:
            printf("Allocation error");
            delete_vector(&vector, cells_amount, &string);
            return allocation_error;
        case end:
            delete_vector(&vector, cells_amount, &string);
            break;
    }
    fclose(file);
    return 0;
}
