#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

double pi_equation(double epsilon){
    double x = 4, x_prev;
    do {
        x_prev = x;
        x -= (cos(x) + 1) / (-sin(x));
    } while (x_prev - x > epsilon);
    return x;
}

double ln2_equation(double epsilon){
    double x = 1, x_prev;
    do {
        x_prev = x;
        x -= (exp(x) - 2) / (exp(x));
    } while (x_prev - x > epsilon);
    return x;
}

double e_equation(double epsilon){
    double x = 3, x_prev;
    do {
        x_prev = x;
        x -= (log(x) - 1) / (1 / x);
    } while (x_prev - x > epsilon);
    return x;
}

double sqrt2_equation(double epsilon){
    double x = 2, x_prev;
    do {
        x_prev = x;
        x -= (x * x - 2) / (2 * x);
    } while (x_prev - x > epsilon);
    return x;
}

double pi_row(double epsilon){
    int n = 1;
    int number_sign = -1;
    double sum = 0, elem = 0;
    do {
        elem = (-1.0) * number_sign / (2 * n - 1);
        sum += elem;
        n++;
        number_sign = -number_sign;
    } while (fabs(elem) > epsilon);
    return (4 * sum);
}

double e_row(double epsilon){
    double sum = 1;
    int n = 0;
    int factorial = 1;
    double elem;
    do {
        n++;
        factorial *= n;
        elem = 1.0 / factorial;
        sum += elem;
    } while (elem > epsilon);
    return sum;
}

double ln2_row(double epsilon){
    int n = 1;
    int number_sign = -1;
    double sum = 0, elem = 0;
    do {
        elem = (-1.0) * number_sign / n;
        sum += elem;
        n++;
        number_sign = -number_sign;
    } while (fabs(elem) > epsilon);
    return sum;
}

double e_limit(double epsilon){
    int n = 1;
    double limit = 0, limit_prev = 0;
    do {
        limit_prev = limit;
        limit = pow((1 + 1.0/n), n);
        n++;
    } while ((limit - limit_prev) > epsilon);
    return limit;
}

double ln2_limit(double epsilon){
    int n = 1;
    double limit = 0, limit_prev = 0;
    do {
        limit_prev = limit;
        limit = n * (pow(2, 1.0/n) - 1);
        n++;
    } while (fabs(limit - limit_prev) > epsilon);
    return limit;
}

double pi_limit(double epsilon){
    int n = 1;
    double limit = 4, limit_prev = 0;
    do {
        n++;
        limit_prev = limit;
        limit *= (4.0 * (n-1) * n) / (4 * n * n - 4 * n + 1);
    } while (fabs(limit - limit_prev) > epsilon);
    return limit;
}

double sqrt2_row(double epsilon){
    int k = 2;
    double product_prev, product = 1;
    do {
        product_prev = product;
        product *= pow(2, pow(2, -k));
        k++;
    } while (product - product_prev > epsilon);
    return product;
}

double sqrt2_limit(double epsilon){
    double limit = -0.5, limit_prev = -0.5;
    do {
        limit_prev = limit;
        limit = limit_prev - limit_prev * limit_prev / 2 + 1;
    } while (fabs(limit - limit_prev) > epsilon);
    return limit;
}

double gamma_row(double epsilon){
    double pi = pi_row(epsilon);
    double k = 2;
    double sum = - (pi * pi / 6), elem, sqrt_elem = 1;
    int square_flag = 4;
    int flag_for_squares = 5;
    do {
        if (k == square_flag){
            sqrt_elem = square_flag;
            square_flag += flag_for_squares;
            flag_for_squares += 2;
            k++;
            continue;
        }
        elem = 1.0 / sqrt_elem - 1.0 / k;
        sum += elem;
        k++;
    } while (fabs(elem) > epsilon);
    return sum;
}

double gamma_limit(double epsilon){
    int m = 1;
    double limit = 0, limit_prev;
    do {
        m++;
        limit_prev = limit;
        limit = 0;
        double logariphm = 0, combinations = m, sum = 0;
        for (int k = 2; k <= m; k++){
            combinations *= (m - k + 1.0) / k;
            logariphm += log(k);
            sum = combinations * logariphm / k;
            if (k % 2 == 1) sum *= -1;
            limit += sum;
        }
    } while (fabs(limit_prev - limit) > epsilon);
    return limit;
}

int isprime(int number, int* prime_numbers, int* prime_count){
    for(int i = 0; i < *prime_count; i++){
        if (number % prime_numbers[i] == 0) return 0;
    }
    prime_numbers[*prime_count] = number;
    (*prime_count)++;
    return 1;
}

double gamma_equation(double epsilon){
    int t = 2;
    double product = 0.5;
    double limit, limit_prev;
    limit = -log(log(t) * product);
    int len = 16;
    int prime_count = 1;
    int* prime_numbers = (int*)malloc(sizeof(int) * len);
    if(!prime_numbers){
        return 1;
    }
    prime_numbers[0] = 2;
    do {
        t++;
        limit_prev = limit;
        if (isprime(t, prime_numbers, &prime_count))
            product *= (t - 1.0) / t;
        if(prime_count == len){
            len *= 2;
            int* ptr = (int*)realloc(prime_numbers, sizeof(int) * len);
            if(!ptr) {
                free (prime_numbers);
                return 1;
            }
            prime_numbers = ptr;
        }
        limit = -log(log(t) * product);
    } while (fabs(limit - limit_prev) > epsilon);
    free(prime_numbers);
    return limit;
}

int is_double(char* number)
{
    int c = 0;
    if (*number == '-') return 0;
    while (*number != '\0'){
        if (*number == '.'){
            if (c == 0) c += 1;
            else return 0;
        }
        else if (!(*number >= '0' && *number <= '9')) return 0;
        number++;
    }
    return 1;
}

int main(int argc, char* argv[]){
    if (argc > 2) {
        printf("too many arguments");
        return 1;
    }
    if (!(is_double(argv[1]))){
        printf("Epsilon must be float");
        return 2;
    }
    double epsilon = atof(argv[1]);
    if (epsilon == 0) {
        printf("Epsilon must be positive");
        return 3;
    }
    printf ("pi with equation = %f\n", pi_equation(epsilon));
    printf ("pi with row = %f\n", pi_row(epsilon));
    printf ("pi with limit = %f\n", pi_limit(epsilon));
    printf ("e with equation = %f\n", e_equation(epsilon));
    printf ("e with row = %f\n", e_row(epsilon));
    printf ("e with limit = %f\n", e_limit(epsilon));
    printf ("ln2 with equation = %f\n", ln2_equation(epsilon));
    printf ("ln2 with row = %f\n", ln2_row(epsilon));
    printf ("ln2 with limit = %f\n", ln2_limit(epsilon));
    printf ("sqrt2 with equation = %f\n", sqrt2_equation(epsilon));
    printf ("sqrt2 with row = %f\n", sqrt2_row(epsilon));
    printf ("sqrt2 with limit = %f\n", sqrt2_limit(epsilon));
    double gamma = gamma_equation(epsilon);
    if(gamma == 1)
        printf("Allocation error for gamma equation");
    else printf ("gamma with equation = %f\n", gamma_equation(epsilon));
    printf ("gamma with row = %f\n", gamma_row(epsilon));
    printf ("gamma with limit = %f\n", gamma_limit(epsilon));
    return 0;
}