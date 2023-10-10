#include <stdio.h>
#include <stdlib.h>
#include <math.h>


int flag_validation(char* flag){
    if (flag[2] != 0) return 0;
    if (flag[0] != '-' && flag[0] != '/') return 0;
    if (flag[1] != 'q' && flag[1] != 'm' && flag[1] != 't') return 0;
    return 1;
}

int arguments_amount_validation(char flag, int argc){
    if(flag == 'q' && argc != 6) return 0;
    if(flag == 'm' && argc != 4) return 0;
    if(flag == 't' && argc != 6) return 0;
    return 1;
}

int real_number_validation(char* number){
    int i = 1;
    char symbol = number[0];
    int dot_counter = 0;
    if (symbol != '-' && (symbol < '0' || symbol > '9')) return 0;
    while (number[i] != 0){
        symbol = number[i];
        if (symbol == '.') {
            dot_counter++;
            if (dot_counter == 2) return 0;
        }
        if (symbol != '.' && (symbol < '0' || symbol > '9')) return 0;
        i++;
    }
    return 1;
}

int integer_number_validation(char* number){
    int i = 1;
    char symbol = number[0];
    if (symbol != '-' && (symbol < '0' || symbol > '9')) return 0;
    while(number[i] != 0){
        symbol = number[i];
        if (symbol < '0' || symbol > '9') return 0;
        i++;
    }
    if (atoi(number) < 1) return 0;
    return 1;
}

int triangle(double a, double b, double c, double epsilon){
    a = a*a;
    b = b*b;
    c = c*c;
    if (a < b){
        if (b < c){
            if (fabs(a + b - c) < epsilon) return 1;
        }else if (fabs(a + c - b) < epsilon) return 1;
    }else if (a < c){
        if (fabs(a + b - c) < epsilon) return 1;
    }else if (fabs(b + c - a) < epsilon) return 1;
    return 0;
}

void swap(double* coefficients, int i, int j){
    double elem = coefficients[i];
    coefficients[i] = coefficients[j];
    coefficients[j] = elem;
}

int permutations(double* coefficients, int amount, double epsilon){
    int j = amount - 2;
    while(j != -1 && coefficients[j] - coefficients[j+1] >= -epsilon) j--;
    if (j == -1) return 0;
    int i = amount - 1;
    while(coefficients[j] - coefficients[i] >= -epsilon) i--;
    swap(coefficients, j, i);
    int k = j+1, l = amount - 1;
    while(k < l) swap(coefficients, k++, l--);
    return 1;
}

/*void sort_coefficients(double *array, int length)
{
    double temp;
    for (int i = 0; i < length - 1; ++i)
    {
        for (int j = 0; j < length - i - 1; ++j){
            if (array[j] > array[j + 1])
                swap(array, j, j+1);
        }
    }
    return;
}*/

int equations_roots(double* roots, double* coefficients, double epsilon){
    double a = coefficients[0];
    double b = coefficients[1];
    double c = coefficients[2];
    if (a == 0){
        if (b == 0){
            return -1;
        }
        roots[0] = -c/b;
        return 1;
    }
    double discriminant = b*b - 4*a*c;
    if(discriminant > epsilon){
        roots[0] = (-b + sqrt(discriminant))/(2*a);
        roots[1] = (-b - sqrt(discriminant))/(2*a);
        return 2;
    }
    if(fabs(discriminant) < epsilon){
        roots[0] = -b / (2*a);
        return 1;
    }
    return 0;
}

int comp (const double * num1, const double * num2) { 
    double eps = 0.00000000001;
    if(*num2 > *num1) return -1;
    else if(*num1 > *num2) return 1;
    else if(fabs(*num1 - *num2)<eps)return 0;
}

int main(int argc, char* argv[]){
    if (!flag_validation(argv[1])){
        printf("Wrong flag detected");
        return 1;
    }
    char flag = argv[1][1];
    if(!arguments_amount_validation(flag, argc)){
        printf("Wrong amount of arguments");
        return 2;
    }
    double epsilon;
    if(flag == 'q' || flag == 't'){
        for (int i = 2; i < argc; i++){
            if (!real_number_validation(argv[i])){
                printf ("Wrong number detected");
                return 3;
            }
        }
        epsilon = atof(argv[2]);
        if (epsilon <= 0){
            printf("Epslon must be positive");
            return 3;
        }
    }else{
        for (int i = 2; i < 4; i++){
            if (!integer_number_validation(argv[i])){
                printf("Wrong number detected");
                return 3;
            }
        }
    }
    int number1;
    switch(flag){
        case 'q':
            epsilon = atof(argv[2]);
            double* coefficients = (double*)malloc(sizeof(double) * 3);
            if (!coefficients){
                printf("Allocation error");
                return 4;
            }
            double* roots = (double*)malloc(sizeof(double) * 2);
            if (!roots){
                printf("Allocation error");
                return 4;
            }
            for(int i = 3; i < 6; i++){
                coefficients[i-3] = atof(argv[i]);
            }
            qsort(coefficients, 3, sizeof(double), (int(*)(const void *, const void *))comp);
            do {
                printf("Roots of equation %fx^2 + %fx + %f:\n", coefficients[0], coefficients[1], coefficients[2]);
                int roots_amount = equations_roots(roots, coefficients, epsilon);
                if (roots_amount == -1) printf("It is not an equation");
                else if (roots_amount == 2) printf("%f\n%f\n", roots[0], roots[1]);
                else if (roots_amount == 1) printf("%f\n", roots[0]);
                else if (roots_amount == 0) printf("This equation has no real roots\n");
            } while(permutations(coefficients, 3, epsilon));
            free(coefficients);
            free(roots);
            break;
        case 'm':
            number1 = atoi(argv[2]);
            int number2 = atoi(argv[3]);
            if (number1 % number2 == 0)
                printf("The first number is a multiple of the second");
            else 
                printf("The first number is not a multiple of the second");
            break;
        case 't':
            epsilon = atof(argv[2]);
            double a = atof(argv[3]);
            double b = atof(argv[4]);
            double c = atof(argv[5]);
            if (triangle(a, b, c, epsilon))
                printf("Parameters can be lengths of triangle");
            else printf("Parameters can not be lengths of triangle");
            break;
    }
    return 0;
}