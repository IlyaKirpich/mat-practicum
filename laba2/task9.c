#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

int is_terminating(double number, int base){
    double epsilon = 1e-9;
    long long int length = 1;
    while ((number - (int)number) > epsilon){
        length *= 10;
        number *=10;
    }
    long long int modulo = (int)number % length;
    modulo *= base;
    for (int i = 0; i < 10; i++){
        modulo %= length;
        if (modulo == 0) return 1;
        modulo *= base;
    }
    return 0;
}

void are_terminating(int* answer, int base, int amount, ...){
    va_list numbers;
    va_start(numbers, amount);
    for (int i = 0; i < amount; i++){
        double number = va_arg(numbers, double);
        answer[i] = is_terminating(number, base);
    }
    va_end(numbers);
}

int main(){
    int amount = 5;
    int* answer = (int*)malloc(sizeof(int) * amount);
    if (!answer){
        printf("Allocation error");
        return -1;
    }
    are_terminating(answer, 5, amount, 0.2, 0.4, 0.3, 0.04, 0.000001024);
    for (int i = 0; i < amount; i++){
        printf("%d ", answer[i]);
    }
    free(answer);
    return 0;
}