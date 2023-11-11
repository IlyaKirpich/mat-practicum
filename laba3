#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef enum{
    string_ok,
    string_allocation_error = -1
}status;

status from_decimal(char** answer, int number, int new_base){
    int r = 0;
    int base = new_base;
    while (base > 1){
        r++;
        base >>= 1;
    }
    if (number == 0){
        *answer = (char*)malloc(sizeof(char) * 2);
        if (!(*answer)) return string_allocation_error;
        (*answer)[0] = '0';
        (*answer)[1] = '\0';
        return string_ok;
    }
    int flag = 0;
    if (number < 0){
        flag = 1;
        number = -number;
    }
    int length = log(number) / log(new_base) + 2 + flag;
    *answer = (char*)malloc(sizeof(char) * length);
    int index = length - 2;
    while (number){
        (*answer)[index] = ((number & (new_base - 1)) < 10) ? (number & (new_base - 1)) + '0' : (number & (new_base - 1)) + 'A' - 10;
        number >>= r;
        index--;
    }
    if (flag) (*answer)[0] = '-';
    (*answer)[length-1] = 0;
    return string_ok;
}


int main(){
    char* answer = NULL;
    if (from_decimal(&answer, 255, 16) == string_allocation_error){
        printf("Allocation error");
        return -1;
    }
    else{
        printf("%s", answer);
    }
    free(answer);
    return 0;
}
