#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>

typedef enum{
    ok,
    allocation_error = -1
} status;

char* column_addition(const int base, char* number1, char* number2);
status variable_summ(char** summ, const int base, const int amount, ...);
void lead_zeros_delete(char* number, int length);

int main(){
    char* result = NULL;
    variable_summ(&result, 10, 3, "9", "1", "10");
    printf("%s", result);
    free(result);
    return 0;
}

void lead_zeros_delete(char* number, int length){
    while(number[0] == '0'){
        for (int i = 0; i < length - 1; i++){
            number[i] = number[i+1];
        }
        length--;
    }
    if (length == 0){
        number[0] = '0';
        number[1] = 0;
    }
    else number[length] = 0;
}

char* column_addition(const int base, char* number1, char* number2){
    int length1 = strlen(number1);
    int length2 = strlen(number2);
    int length;
    if (length1 > length2) length = length1 + 2;
    else length = length2 + 2;
    char* answer = (char*)malloc(sizeof(char) * length);
    if (!answer) return NULL;
    int index = length - 2;
    int index1 = length1 - 1;
    int index2 = length2 - 1;
    int digit = 0;
    int inmind = 0;
    while (index1 >= 0 || index2 >= 0){
        if (index1 >= 0)
            digit += isdigit(number1[index1])? number1[index1]- '0': toupper(number1[index1]) - 'A' + 10;
        if (index2 >= 0)
            digit += isdigit(number2[index2])? number2[index2]- '0': toupper(number2[index2]) - 'A' + 10;
        digit += inmind;
        inmind = 0;
        if (digit >= base){
            digit -= base;
            inmind++;
        }
        answer[index] = digit < 10 ? digit + '0' : digit + 'A' - 10;
        digit = 0;
        index1--;
        index2--;
        index--;
    }
    answer[index] = inmind + '0';
    answer[length-1] = 0;
    return answer;
}

status variable_summ(char** summ, const int base, const int amount, ...){
    *summ = malloc(sizeof(char) * 2);
    if (!(*summ)) return allocation_error;
    (*summ)[0] = '0';
    (*summ)[1] = 0;
    va_list numbers;
    va_start(numbers, amount);
    for (int i = 0; i < amount; i++){
        char* number = va_arg(numbers, char*);
        *summ = column_addition(base, *summ, number);
        if (*summ == NULL) return allocation_error;
    }
    lead_zeros_delete(*summ, strlen(*summ));
    return ok;
} 