#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

typedef enum {
    string_ok,
    string_allocation_error = -1
} status;

status reexpansion(const double a, double** new_coef, int power, ...);

int main(){
    double* coef = NULL;
    status a = reexpansion(1, &coef, 3, 1.0, 1.0, 1.0, 1.0);
    if (a == string_allocation_error){
        printf("Allocation error!!!!!!!!!!!!!!!");
        return -1;
    }
    for(int i = 0; i < 4; i++){
        printf("%f ", coef[i]);
    }
    free(coef);
    return 0;
}

status reexpansion(double a, double** new_coef, int power, ...){
    power++;
    *new_coef = (double*)malloc(sizeof(double) *(power));
    if (!(*new_coef)) return string_allocation_error;
    double coefs[power];
    va_list coefficient;
    va_start(coefficient, power);
    for (int i = 0; i < power; i++){
        (*new_coef)[i] = va_arg(coefficient, double);
    }
    va_end(coefficient);
    double factorial = 1;
    double a_power = a;
    for (int i = 0; i < power - 1; i++){
        for (int j = i+1; j < power; j++){
            (*new_coef)[i] /= factorial;
            (*new_coef)[i] += (a_power * (*new_coef)[j])/factorial;
            (*new_coef)[j] *= (j-i) / factorial;
            a_power *= a;
        }
        a_power = a;
        factorial *= (i + 1);
    }
    (*new_coef)[power-1] *= (power-2)/factorial;
    return string_ok;
}
