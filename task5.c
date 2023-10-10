#include <stdio.h>
#include <math.h>
#include <stdlib.h>

double summ_a(double epsilon, double x){
    double sum = 1 + x;
    int n = 2;
    double elem = x, elem_prev;
    do {
        elem_prev = elem;
        elem *= x / n;
        if (fabs(n - x) < epsilon){
            elem_prev = 0;
        }
        n++;
        sum += elem;
    } while (fabs(elem - elem_prev) > epsilon);
    return sum;
}

double summ_b(double epsilon, double x){
    double sum = 1;
    int n = 1;
    double elem = 1, elem_prev;
    do {
        elem_prev = elem;
        elem *= -(x * x / (4*n*n - 2*n));
        n++;
        sum += elem;
    } while (fabs(elem - elem_prev) > epsilon);
    return sum;
}

double summ_c(double epsilon, double x){
    double sum = 1;
    int n = 1;
    double elem = 1, elem_prev;
    do {
        elem_prev = elem;
        elem *= 9*n*n*x*x / (9*n*n-9*n+2);
        n++;
        sum += elem;
    } while (fabs(elem - elem_prev) > epsilon);
    return sum;
}

double summ_d(double epsilon, double x){
    double sum = -x*x/2;
    int n = 2;
    double elem = -x*x/2, elem_prev;
    do {
        elem_prev = elem;
        elem *= -((2*n-1)*x*x) / (2*n);
        n++;
        sum += elem;
    } while (fabs(elem - elem_prev) > epsilon);
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
    if (argc > 3) {
        printf("Too many arguments");
        return 0;
    }
    if (argc < 3){
        printf("Too few arguments");
        return 0;
    }
    if (!(is_double(argv[1]))){
        printf("Epsilon must be float and positive");
        return 0;
    }
    if (!(is_double(argv[2]))){
        printf("x must be float and positive");
        return 0;
    }
    double epsilon = atof(argv[1]);
    double x = atof(argv[2]);
    printf("Summ a = %f\n", summ_a(epsilon, x));
    printf("Summ b = %f\n", summ_b(epsilon, x));
    if (fabs(x) > 1){
        printf("x must be < 1 for third and fourth summ");
    } else{
        printf("Summ c = %f\n", summ_c(epsilon, x));
        printf("Summ d = %f\n", summ_d(epsilon, x));
    }
}