#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>

double polynomial_calculator(double point, int power, ...){
    power++;
    double result = 0;
    va_list number;
    va_start(number, power);
    for (int i = 0; i < power; i++){
        result = result * point + va_arg(number, double);
    }
    va_end(number);
    return result;
}

enum status{
    convex = 1,
    not_convex = 0,
    too_few_arguments = -1,

};

int sign_func(double x){
    if (x > 0) return 1;
    if (x < 0) return -1;
    return 0;
}

int convex_polygon(int amount, ...){
    amount *= 2;
    va_list points;
    va_start(points, amount);
    if (amount < 6) return too_few_arguments;
    double x_first = va_arg(points, double);
    double y_first = va_arg(points, double);
    double x = x_first, y = y_first;
    double x_new = va_arg(points, double), y_new = va_arg(points, double);
    double new_x_vector = x_new - x;
    double new_y_vector = y_new - y;
    double x_vector, y_vector;
    for (int i = 0; i < amount/2 - 2; i++){
        x = x_new;
        y = y_new;
        x_vector = new_x_vector;
        y_vector = new_y_vector;
        x_new = va_arg(points, double);
        y_new = va_arg(points, double);
        new_x_vector = x_new - x;
        new_y_vector = y_new - y;
        if (x_vector * new_y_vector - y_vector * new_x_vector >= 0)
            return not_convex;
    }
    x_vector = new_x_vector;
    y_vector = new_y_vector;
    new_x_vector = x_first - x_new;
    new_y_vector = y_first - y_new;
    if (x_vector * new_y_vector - y_vector * new_x_vector >= 0)
        return not_convex;
    return convex;
}

int main(){
    printf("%f\n", polynomial_calculator(2.0, 3, 0.0, 0.0, 0.0, 19.0));
    printf("%d", convex_polygon(5, 1.0, 1.0, 1.0, -1.0, -1.0, -1.0, -2.0, 0.0, 0.0, 2.0));
}