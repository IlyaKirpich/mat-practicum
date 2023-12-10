#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>

double my_pow (double base, int power) {
    if (power < 0) {
        base = 1.0 / base;
        power = fabs(power);
    }
    if (power == 0) return 1;
    if (power % 2 == 1) return my_pow(base, power - 1) * base;
    else {
        double result = my_pow(base, power / 2);
        return result * result;
    }
}

double geometric_average(int amount, ...) {
    double result = 1.0;
    va_list number;
    va_start(number, amount);
    for (int i = 0; i < amount; i++) {
        result *= va_arg(number, double);
    }
    va_end(number);
    if (amount % 2 == 0 && result < 0) return -1.0;
    else result = pow(result, (1.0 / amount));
    return result;
}

int main(int argc, char * argv[]) {
    printf("%f\n", geometric_average(5, 1.0, 2.0, 3.0, 4.0, 5.0));
    printf("%f\n", my_pow(3, -2));
    return 0;
}