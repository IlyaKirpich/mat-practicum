#include <stdio.h>
#include <stdlib.h>
#include <math.h>


int real_number_validation(char* number){
    int i = 0;
    if (number[i] != '-' && (number[i] < '0' || number[i] > '9'))
        return 0;
    i++;
    while(number[i] != 0){
        if (number[i] < '0' || number[i] > '9') return 0;
        i++;
    }
    return 1;
}

void max_min_swap_function(int* vector, int array_size){
    int index_max = 0, index_min = 0;
    int max_value = vector[0], min_value = vector[0];
    for (int i = 1; i < array_size; i++){
        if (vector[i] > max_value){
            max_value = vector[i];
            index_max = i;
        } else if (vector[i] < min_value){
            min_value = vector[i];
            index_min = i;
        }
    }
    vector[index_max] = min_value;
    vector[index_min] = max_value;
}

void make_vector(int* vector, int lower_bound, int upper_bound, int array_size){
    int range = upper_bound - lower_bound + 1;
    for(int i = 0; i < array_size; i++){
        vector[i] = rand() % range + lower_bound;
    }
}

void make_vector_c(int* array_a, int* array_b, int* array_c, int array_size){
    int a_value, closest_value, difference;
    for(int i = 0; i < array_size; i++){
        a_value = array_a[i];
        closest_value = array_b[0];
        difference = fabs(a_value - closest_value);
        for(int j = 1; j < array_size; j++){
            if (fabs(a_value - array_b[j]) < difference){
                difference = fabs(a_value - array_b[j]);
                closest_value = array_b[j];
            }
        }
        array_c[i] = a_value + closest_value;
    }
}

int main(int argc, char* argv[]){
    if (argc != 3){
        printf("Wrong amount of arguments");
        return 1;
    }
    if (!real_number_validation(argv[1])){
        printf("Wrong first number");
        return 2;
    }
    if (!real_number_validation(argv[2])){
        printf("Wrong second number");
        return 2;
    }
    int lower_bound = atoi(argv[1]);
    int upper_bound = atoi(argv[2]);
    int array_first[10];
    make_vector(array_first, lower_bound, upper_bound, 10);
    for(int i = 0; i < 10; i++){
        printf("%d ", array_first[i]);
    }
    printf("\n");
    max_min_swap_function(array_first, 10);
    for(int i = 0; i < 10; i++){
        printf("%d ", array_first[i]);
    }
    int array_size = rand() % 9991 + 10;
    int* array_a = (int*)malloc(sizeof(int) * array_size);
    if (!array_a){
        printf("Allocation error");
        return 3;
    }
    int* array_b = (int*)malloc(sizeof(int) * array_size);
    if (!array_b){
        printf("Allocation error");
        return 3;
    }
    int* array_c = (int*)malloc(sizeof(int) * array_size);
    if (!array_c){
        printf("Allocation error");
        return 3;
    }
    make_vector(array_a, -1000, 1000, array_size);
    make_vector(array_b, -1000, 1000, array_size);
    make_vector_c(array_a, array_b, array_c, array_size);
    free(array_a);
    free(array_b);
    free(array_c);
    return 0;
}