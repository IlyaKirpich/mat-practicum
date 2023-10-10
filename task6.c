#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double integral_d(double epsilon){
    double n = 2;
    double summ = 1;
    double summ_prev;
    int flag = -1;
    do {
        summ_prev = summ;
        summ += flag*(pow(n, -n));
        n++;
        flag = -flag;
    } while (fabs(summ - summ_prev) > epsilon);
    return summ;
}

double integral_c(double epsilon){
    int n = 1;
    double sum_prev, sum = 0;
    do {
        sum_prev = sum;
        sum += 1.0 / (n*n + n);
        n++;
    } while (sum - sum_prev > epsilon);
    return sum;
}

double integral_a(double epsilon){
    int n = 2;
    double sum_prev, sum = 1;
    int flag = -1;
    do {
        sum_prev = sum;
        sum += flag* (1.0 / (n*n));
        n++;
        flag = -flag;
    } while (fabs(sum - sum_prev) > epsilon);
    return sum;
}

double integral_b(double epsilon){
    int n = 1;
    double sum_prev, sum = 1;
    int flag = - 1;
    long long multiplier = 2;
    do {
        sum_prev = sum;
        sum += flag * 1.0 / ((2*n +1) * multiplier);
        flag = -flag;
        n++;
        multiplier *= 2 * n;
    } while (fabs(sum - sum_prev) > epsilon);
    return sum;
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
    if (argc > 2){
        printf("Too many arguments");
        return 0;
    }
    if(!(is_double(argv[1]))){
        printf ("Epsilon must be float");
        return 0;
    }
    double epsilon = atof(argv[1]);
    printf("integral a = %f\n", integral_a(epsilon));
    printf("integral b = %f\n", integral_b(epsilon));
    printf("integral c = %f\n", integral_c(epsilon));
    printf("integral d = %f\n", integral_d(epsilon));
}