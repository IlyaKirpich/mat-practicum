#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double equation(double x){
    return sqrt(x) - 2;
}

double dichotomy(double left, double right, double epsilon, double (*f)(double)){
    double x;
    while (f(right) - f(left) > epsilon){
        x = (left + right) / 2;
        if (f(x) * f(left) <= epsilon) right = x;
        else left = x;
    }
    return x;
}

int main(){
    printf("%f", dichotomy(3, 5, 0.00001, equation));
}