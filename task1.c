#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void multiples (int* array_multiples, int value, int multiples_count){
    array_multiples[0] = value;
    for (int i = 1; i < multiples_count; ++i){
        array_multiples[i] = array_multiples[i-1] + value;
    }
    return;
}

enum prime_number_status_codes
{
    pnsc_prime,
    pnsc_notprime,
    pnsc_one
};

enum prime_number_status_codes prime_number(int value){
    if (value < 2)
        return pnsc_one;
    if (value == 2)
        return pnsc_notprime;
    for (int i = 3; i < value; i+=2){
        if (value % i != 0)
            continue;
        return pnsc_notprime;
    }
    return pnsc_prime;
}

void number_splitter(int* result, int number, int digit_count){
    int i = 0;
    while(digit_count != 0){
        result[digit_count-1] = number % 10;
        number /= 10;
        digit_count--;
    }
}

int sum_natural(int number){
    return ((1 + number)/2 * number);
}

enum factorial_status_codes
{
    fsc_ok,
    fsc_overflow,
    fsc_invalid_parameter
};

enum factorial_status_codes factorial(int value, long *result){
    enum factorial_status_codes recursive_status_code;
    if (value > 20){
        return fsc_invalid_parameter;
    }
    if (value == 0 || value == 1){
        *result = 1;
        return fsc_ok;
    }
    recursive_status_code = factorial(value - 1, result);
    if (recursive_status_code == fsc_ok){
        *result *= value;
    }
    return recursive_status_code;
}

void power_table(int value, long** table)
{
    for (int i = 0; i < 10; i++)
    {
        table[i][0] = i + 1;
        for (int j = 1; j < value; j++){
            table[i][j] = table[i][j-1] * (i+1);
        }
    }
    return;
}



int main(int argc, char* argv[]){
    if (argv[2][0] != '-' && argv[2][0] != '/' ||
    argv[2][1] != 'h' && argv[2][1] != 'p' && argv[2][1] != 's' 
    && argv[2][1] != 'e' && argv[2][1] != 'a' && argv[2][1] != 'f' || strlen(argv[2]) != 2)
    {
        printf("Wrong flag detected");
        return 0;
    }
    if (atoi(argv[1]) == 0){
        printf("Wrong number detected");
        return 0;
    }
    int number = atoi(argv[1]);
    if (argv[2][1] == 'h'){
        int multiples_count = 100 / number;
        if (multiples_count == 0)
        {
            printf("Theres no multiples < 100");
            return 0;
        }
        int* multiples_result = (int*)malloc(sizeof(int) * multiples_count);
        multiples(multiples_result, number, multiples_count);
        for (int i = 0; i < multiples_count; i++){
            printf("%d ", multiples_result[i]);
        }
        free(multiples_result);
        return 0;
    }
    if (argv[2][1] == 'p'){
        switch(prime_number(number))
        {
            case pnsc_one:
                printf("Number = 1");
                break;
            case pnsc_prime:
                printf("%d is prime", number);
                break;
            case pnsc_notprime:
                printf("%d is not prime", number);
                break;
        }
        return 0;
    }
    if (argv[2][1] == 's'){
        int digit_count = 0;
        int number_for_count = number;
        while (number_for_count != 0){
            digit_count++;
            number_for_count /= 10;
        }
        int* digit_vector = (int*)malloc(sizeof(int) * digit_count);
        number_splitter(digit_vector, number, digit_count);
        for (int i = 0; i < digit_count; i++){
            printf("%d ", digit_vector[i]);
        }
        free(digit_vector);
        return 0;
    }
    if (argv[2][1] == 'e'){
        if (number > 10){
            printf("Invalid parameter");
            return 0;
        }
        long** table = (long**)malloc(sizeof(long*) * 10);
        for(int i = 0; i < 10; i++)
            table[i] = (long*)malloc(sizeof(long) * number);
        power_table(number, table);
        for (int i = 0; i < 10; i++){
            for (int j = 0; j < number; j++){
                printf("%ld ", table[i][j]);
            }
            printf("\n");
        }
        for (int i = 0; i < 10; i++){
            free(table[i]);
        }
        free(table);
        return 0;
    }
    if (argv[2][1] == 'a'){
        printf("%d", sum_natural(number));
    }
    if (argv[2][1] == 'f'){
        long result;
        switch(factorial(number, &result)){
            case fsc_invalid_parameter:
                printf("Invalid parameter detected");
                break;
            case fsc_overflow:
                printf("Overflow detected");
                break;
            case fsc_ok:
                printf("%d! = %ld", number, result);
        }
    }
}
