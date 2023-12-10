#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

enum status{
    string_ok = 0,
    wrong_flag = -1,
    wrong_arguments_amount = -2,
    string_allocation_error = -3,
    wrong_seed = -4
};

int string_length(char* string){
    int length = 0;
    while(string[length] != 0) length++;
    return length;
    /*
    while (*string != '\0') {
        string ++;
        length ++;
    }
    */
}

int flag_validation(char* flag){
    if (string_length(flag) != 2) return wrong_flag;
    if (flag[0] != '-') return wrong_flag;
    if (flag[1] != 'l' && flag[1] != 'r' && flag[1] != 'u' && flag[1] != 'n' && flag[1] != 'c')
        return wrong_flag;
    return string_ok;
}

int get_reversed_string(char* string, char* new_string, int length){
    for (int i = 0; i < length; i++){
        new_string[i] = string[length - i - 1];
    }
    new_string[length] = 0;
    return string_ok;
}

int r_flag(char* string, int length){
    char* new_string = (char*)malloc(sizeof(char) * (length+1));
    if (!new_string){
        printf("Allocation error");
        return string_allocation_error;
    }
    get_reversed_string(string, new_string, length);
    printf("Your string: %s\nReversed string: %s", string, new_string);
    free(new_string);
    return string_ok;
}

int get_upper_string(char* string, char* new_string, int length){
    for (int i = 0; i < length; i++){
        if (i % 2 == 0){
            new_string[i] = string[i];
        } else {
            new_string[i] = toupper(string[i]);
        }
    }
    new_string[length] = 0;
    return string_ok;
}

int u_flag(char* string, int length){
    char* new_string = (char*)malloc(sizeof(char) * (length+1));
    if (!new_string){
        printf("Allocation error");
        return string_allocation_error;
    }
    get_upper_string(string, new_string, length);
    printf("Your string: %s\nNew string: %s", string, new_string);
    free(new_string);
    return string_ok;
}

/*int string_comparator(const void* a, const void* b){
    char arg1 = *(const char*)a;
    char arg2 = *(const char*)b;
    if (isdigit(arg1)){
        if (isdigit(arg2)) 
            return a < b;
        return -1;
    }
    if (isalpha(arg1)){
        if (isdigit(arg2)) return 1;
        if (isalpha(arg2)) return a < b;
        return -1;
    }
    if (isdigit(arg2)) return 1;
    if (isalpha(arg2)) return 1;
    return a < b;
}*/

void string_sort(char* string, char* new_string, int length){
    int digit_count = 0;
    int letter_count = 0;
    int symbol_count = 0;
    for (int i = 0; i < length; i++){
        if (isdigit(string[i])) digit_count++;
        else {
            if (isalpha(string[i])) letter_count++;
            else symbol_count++;
        }
    }
    int digit_index = 0;
    int letter_index = digit_count;
    int symbol_index = digit_count+letter_count;
    for (int i = 0; i < length; i++){
        if (isdigit(string[i])){
            new_string[digit_index] = string[i];
            digit_index++;
        } else if (isalpha(string[i])){
            new_string[letter_index] = string[i];
            letter_index++;
        } else {
            new_string[symbol_index] = string[i];
            symbol_index++;
        }
    }
    new_string[length] = 0;
}

int n_flag(char* string, int length){
    char* new_string = (char*)malloc(sizeof(char) * (length+1));
    if (!new_string){
        printf("Allocation error");
        return string_allocation_error;
    }
    string_sort(string, new_string, length);
    printf("Your string: %s\nSorted string: %s", string, new_string);
    free(new_string);
    return string_ok;
}

enum status string_concatenation(char** first_string, char* second_string){
    int length1 = string_length(*first_string);
    int length2 = string_length(second_string);
    char* ptr = realloc(*first_string, sizeof(char) * (length1+length2+1));
    if (!ptr) return string_allocation_error;
    *first_string = ptr;
    for (int i = 0; i < length2; i++){
        (*first_string)[length1+i] = second_string[i];
    }
    (*first_string)[length1+length2] = '\0';
    length1 = string_length(*first_string);
    return string_ok;
}

void order_array(int* array, int string_amount, int seed){
    srand(seed);
    int new_number;
    int unique = 1;
    for (int i = 0; i < string_amount;){
        new_number = rand() % string_amount;
        for (int j = 0; j < i; j++){
            if (new_number == array[j]){
                unique = 0;
                break;
            }
        }
        if (unique == 0)
            unique = 1;
        else{
            array[i] = new_number;
            i++;
        }
    }
}

enum status seed_validation(char* seed){
    int i = 0;
    while (seed[i] != 0){
        if (!isdigit(seed[i])) return wrong_seed;
        i++;
    }
    return string_ok;
}

enum status c_flag(char** argv, int argc){
    if (seed_validation(argv[3]) == wrong_seed){
        printf("Wrong seed detected");
        return wrong_seed;
    }
    int seed = atoi(argv[3]);
    int string_amount = argc - 3;
    int* array = (int*)malloc(sizeof(int)* string_amount);
    if(!array){
        printf("Allocation error");
        return string_allocation_error;
    }
    order_array(array, string_amount, seed);
    for (int i = 0; i < string_amount; i++){
        if(array[i] == 0) array[i] += 2;
        else array[i]+=3;
    }
    int length = string_length(argv[array[0]]);
    char* result_string = (char*)malloc(sizeof(char)* (length+1));
    result_string[length] = 0;
    for (int i = 0; i < length; i++) 
        result_string[i] = argv[array[0]][i];
    for (int i = 1; i < string_amount; i++){
        if(string_concatenation(&result_string, argv[array[i]]) == string_allocation_error){
            printf("Allocation error");
            return string_allocation_error;
        }
    }
    printf("%s", result_string);
}

int main(int argc, char* argv[]){
    if (argc < 3){
        printf("Wrong amount of arguments");
        return wrong_arguments_amount;
    }
    if (flag_validation(argv[1]) == wrong_flag){
        printf("Wrong flag detected");
        return wrong_flag;
    }
    char flag = argv[1][1];
    int result;
    if (flag != 'c'){
        if (argc != 3){
            printf("Wrong amount of arguments");
            return wrong_arguments_amount;
        }
    }
    else if (argc < 4){
        printf("Wrong amount of arguments");
        return wrong_arguments_amount;
    }
    int length = string_length(argv[2]);
    switch(flag){
        case 'l':
            printf("Length of your string is %d", length);
            return string_ok;
        case 'r':
            result = r_flag(argv[2], length);
            return result;
        case 'u':
            result = u_flag(argv[2], length);
            return result;
        case 'n':
            result = n_flag(argv[2], length);
            return result;
//qsort(argv[2], length, sizeof(char), string_comparator);
        case 'c':
            result = c_flag(argv, argc);
            return result;
    }
}
