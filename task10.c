#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

typedef enum {
    ok,
    wrong_argument,
    allocation_error
} status;

void skip_delimeter(char* a){
    *a = getchar();
    while (*a == ' ' || *a == '\t' || *a == '\n'){
        *a = getchar();
    }
}

status base_validation(char* base_string, int* base){
    char* ptr = base_string;
    while (*ptr){
        if (!isdigit(*ptr)) return wrong_argument;
        ++ptr;
    }
    *base = atoi(base_string);
    if ((*base) < 2 || (*base) > 36) return wrong_argument;
    return ok;
}

status number_validation(char* number, const int base){
    char* ptr = number;
    if (*ptr == '-') ptr++;
    if (base < 10)
        while (*ptr){
            if (!isdigit(*ptr)) return wrong_argument;
            if (((*ptr) - '0') >= base) return wrong_argument;
            ++ptr;
        }
    else while(*ptr){
        if (isdigit(*ptr)){
            ++ptr;
            continue;
        }
        if (!isalpha(*ptr)) return wrong_argument;
        if (toupper(*ptr) != *ptr) return wrong_argument;
        if ((*ptr) - 'A' + 10 >= base) return wrong_argument;
        ++ptr;
    }
    return ok;
}

status scan_string(char** string){
    *string = (char*) malloc(sizeof(char) * 2);
    int capacity = 2;
    int string_length = 0;
    if (!(*string)) return allocation_error;
    char symbol;
    skip_delimeter(&symbol);
    while (symbol != ' ' && symbol != '\t' && symbol != '\n'){
        (*string)[string_length] = symbol;
        string_length++;
        if (string_length == capacity){
            capacity *= 2;
            char* ptr = realloc((*string), sizeof(char) * capacity);
            if (!ptr){
                free(*string);
                return allocation_error;
            }
            *string = ptr;
        }
        symbol = getchar();
    }
    (*string)[string_length] = '\0';
    (*string) = realloc(*string, sizeof(char) * (string_length+1));
    return ok;
}

status to_decimal(long long *number, char* string_number, const int base){
    *number = 0;
    int sign = (string_number[0] == '-') ? -1 : 1;
    char* ptr = string_number;
    if (sign == -1) ++ptr;
    *number += (isdigit(*ptr))? (*ptr) - '0' : (*ptr) - 'A' + 10;
    ++ptr;
    while (*ptr) {
        (*number) *= base;
        *number += (isdigit(*ptr))? (*ptr) - '0' : (*ptr) - 'A' + 10;
        ++ptr;
    }
    if (*number < 0){
        return wrong_argument;
    }
    (*number) *= sign;
    return ok;
}

status from_decimal(long long number, const int base, char** new){
    if (number == 0){
        *new = (char*)malloc(sizeof(char) * 2);
        if (!(*new)) return allocation_error;
        (*new)[0] = '0';
        (*new)[1] = '\0';
        return ok;
    }
    int sign = (number < 0)? 1:0;
    if (number < 0) number = -number;
    int length = log(number) / log(base) + sign + 2; 
    *new = (char*)malloc(sizeof(char) * length);
    if (!(*new)) return allocation_error;
    (*new)[length-1] = '\0';
    for (int i = length - 2; i >= sign; i--){
        (*new)[i] = (number % base <= 9)? number % base + '0' : number % base + 'A' - 10;
        number /= base;
    }
    if (sign == 1) (*new)[0] = '-';
    return ok;
}

status get_base(int* base){
    char* base_string = NULL;
    if (scan_string(&base_string) != ok) return allocation_error;
    if (base_validation(base_string, &(*base)) != ok) {
        free(base_string);
        return wrong_argument;
    }
    free(base_string);
    return ok;
}

status function(const int base, char** max, char** in9, char** in18, char** in27, char** in36){
    char* string = NULL;
    status now = scan_string(&string);
    if (now != ok) return allocation_error;
    long long int_max = 0;
    long long number = 0;
    while (strcmp(string, "Stop") != 0){
        if (number_validation(string, base)!= ok) {
            free(string);
            return wrong_argument;
        }
        if (to_decimal(&number, string, base)!= ok){
            free(string);
            return wrong_argument;
        }
        if (number > int_max) {
            int_max = number;
        }
        string = NULL;
        if (scan_string(&string)!= ok) return allocation_error;
    }
    if (from_decimal(int_max, base, &(*max)) != ok) return allocation_error;
    if (from_decimal(int_max, 9, &(*in9)) != ok) return allocation_error;
    if (from_decimal(int_max, 18, &(*in18)) != ok) return allocation_error;
    if (from_decimal(int_max, 27, &(*in27)) != ok) return allocation_error;
    if (from_decimal(int_max, 36, &(*in36)) != ok) return allocation_error;
    return ok;
}

int main(){
    printf("Enter base from 2 to 36: ");
    int base = 0;
    status status_func = get_base(&base);
    if (status_func == allocation_error){
        printf("Allocation error");
        return -1;
    }
    if (status_func == wrong_argument){
        printf("Wrong input");
        return -2;
    }
    printf("Now enter your numbers; to stop enter 'Stop':\n");
    char* max = NULL;
    char* in9 = NULL;
    char* in18 = NULL;
    char* in27 = NULL;
    char* in36 = NULL;
    status_func = function(base, &max, &in9, &in18, &in27, &in36);
    if (status_func == allocation_error){
        printf("\nAllocation error");
        return -1;
    }
    if (status_func == wrong_argument){
        printf("\nWrong input");
        return -2;
    }
    printf("\nMax number: %s\nwith base=9: %s\nwith base=18: %s\nwith base=27: %s\nwith base=36: %s\n", max, in9, in18, in27, in36);
    return 0;
} 