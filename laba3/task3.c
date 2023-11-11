#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

typedef enum{
    string_ok,
    end,
    string_allocation_error = -1,
    wrong_flag = -2,
    wrong_string = -3
} status;

typedef struct{
    unsigned int id;
    char name[BUFSIZ];
    char surname[BUFSIZ];
    double salary;
} Employee;

int comp(const void* a, const void* b);
status flag_validation(char* flag);
status id_validation(char* new_id);
status employee_vector(FILE* file, Employee** database, int* database_size, int* employees_amount);
status new_employee(FILE* file, Employee* new);


int main(int argc, char* argv[]){
    if (argc != 4){
        printf("Wrong amount of arguments");
        return -1;
    }
    if (flag_validation(argv[1])!= string_ok){
        printf("Wrong flag");
        return -2;
    }
    char flag = argv[1][1];
    FILE* file = fopen(argv[2], "r");
    FILE* file2 = fopen(argv[3], "w");
    if (!file || !file2){
        printf("Wrong file");
        return -3;
    }
    Employee* vector = (Employee*)malloc(sizeof(Employee) * 2);
    int vector_length = 2;
    int employees_amount = 0;
    status value = employee_vector(file, &vector, &vector_length, &employees_amount);
    if (value == string_allocation_error){
        printf("Allocation error");
        return -4;
    }
    if (value == wrong_string){
        printf("Wrong data");
        return -5;
    }
    qsort(vector, employees_amount, sizeof(Employee), (int(*) (const void *, const void *)) comp);
    switch(flag){
        case 'a':
            for (int i = 0; i < employees_amount; i++){
                printf("%d %s %s %f\n", vector[i].id, vector[i].name, vector[i].surname, vector[i].salary);
            }
            break;
        case 'd':
            employees_amount--;
            for (; employees_amount >= 0; employees_amount--){
                printf("%d %s %s %f\n", vector[employees_amount].id, vector[employees_amount].name, vector[employees_amount].surname, vector[employees_amount].salary);
            }
            break;
    }
    free(vector);
    return 0;
}

int comp(const void* a, const void* b){
    const Employee* first = (const Employee*)a;
    const Employee* second = (const Employee*)b;
    if (first->salary > second->salary) return 1;
    else if (first->salary < second->salary) return -1;
    int flag = strcmp(first->surname, second->surname);
    if (flag > 0) return 1;
    else if (flag < 0) return -1;
    flag = strcmp(first->name, second->name);
    if (flag > 0) return 1;
    else if (flag < 0) return -1;
    if (first->id > second->id) return 1;
    return -1;
}

status flag_validation(char* flag){
    if (strlen(flag) != 2) return wrong_flag;
    if (flag[0] != '-' && flag[0] != '/') return wrong_flag;
    if (flag[1] != 'a' && flag[1] != 'd') return wrong_flag;
    return string_ok;
}

status id_validation(char* new_id){
    char* ptr = new_id;
    while (*ptr){
        if (!isdigit(*ptr)) return wrong_string;
        ++ptr;
    }
    return string_ok;
}

status string_validation(char* new_string){
    char* ptr = new_string;
    while(*ptr){
        if (!isalpha(*ptr)) return wrong_string;
        ++ptr;
    }
    return string_ok;
}

status salary_validation(char* new_string){
    char* ptr = new_string;
    int point_counter = 0;
    while(*ptr){
        if (!isdigit(*ptr) && (*ptr)!= '.') return wrong_string;
        if ((*ptr) == '.') point_counter++;
        ++ptr;
    }
    if (point_counter > 1) return wrong_string;
    return string_ok;
}

status employee_vector(FILE* file, Employee** database, int* database_size, int* employees_amount){
    Employee new;
    status flag;
    flag = new_employee(file, &new);
    while (flag != end){
        if (flag == wrong_string){
            return wrong_string;
        }
        if (flag == string_ok){
            (*database)[*employees_amount] = new;
            (*employees_amount)++;
            if (*employees_amount == *database_size){
                (*database_size) *= 2;
                Employee* ptr = realloc(*database, sizeof(Employee) * (*database_size));
                if (!ptr) {
                    free(database);
                    return string_allocation_error;
                }
                *database = ptr;
            }
        }
        flag = new_employee(file, &new);
    }
    return string_ok;
}

status new_employee(FILE* file, Employee* new){
    char new_id[BUFSIZ];
    char new_name[BUFSIZ];
    char new_surname[BUFSIZ];
    char new_salary[BUFSIZ];
    if (fscanf(file, "%s", new_id) == EOF) return end;
    if (id_validation(new_id) != string_ok) return wrong_string;
    (*new).id = atoi(new_id);
    if (fscanf(file, "%s", new_name) == EOF) return end;
    if (string_validation(new_name) != string_ok) return wrong_string;
    strcpy((*new).name, new_name);
    if (fscanf(file, "%s", new_surname) == EOF) return end;
    if (string_validation(new_surname) != string_ok) return wrong_string;
    strcpy((*new).surname, new_surname);
    if (fscanf(file, "%s", new_salary) == EOF) return end;
    if (salary_validation(new_salary) != string_ok) return wrong_string;
    (*new).salary = atof(new_salary);
    return string_ok;
}