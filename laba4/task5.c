#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


typedef struct Stack {
    int data;
    struct Stack* next;
} Stack;

typedef enum { 
    ok,
    allocation_error,
    end,
    wrong_balance,
    wrong_symbol,
    wrong_power
} status;

int get_priority(char symbol){
    if (symbol == '(' || symbol == ')') return 1;
    if (symbol == '+' || symbol == '-') return 2;
    if (symbol == '*' || symbol == '/' || symbol == '%') return 3;
    if (symbol == '^') return 4;
    return 5;
}

status push(Stack** stack, int value){
    Stack* new = (Stack*)malloc(sizeof(Stack));
    if (!new) return allocation_error;
    new->data = value;
    new->next = *stack;
    *stack = new;
    return ok;
}

char pop(Stack** stack){
    Stack* out;
    out = *stack;
    *stack = (*stack)->next;
    int value = out->data;
    free(out);
    return value;
}

void delete_stack(Stack** stack){
    Stack* out;
    while (*stack != NULL){
        out = *stack;
        *stack = (*stack)->next;
        int value = out->data;
        free(out);
    }
}

int is_delimiter(char symbol){
    if (symbol == ' ' || symbol == '\t') return 1;
    return 0;
}

int is_operand(char symbol){
    if (symbol == '*' || symbol == '^' || symbol == '(' || symbol == '-' || 
    symbol == '+' || symbol == '/' || symbol == '%' || symbol == ')') return 1;
    return 0;
}

status scan_string(FILE* file, char** string){
    char symbol = fgetc(file);
    int index = 0;
    int capacity = 2;
    *string = (char*)malloc(sizeof(char)* 2);
    if (!(*string)) return allocation_error;
    while(symbol != '\n' && symbol != EOF){
        if (!is_delimiter(symbol)){
            (*string)[index] = symbol;
            index++;
            if (index+1 == capacity){
                capacity *= 2;
                char* ptr = realloc(*string, sizeof(char) * capacity);
                if (!ptr) {
                    free(*string);
                    return allocation_error;
                }
                *string = ptr;
            }
        }
        symbol = fgetc(file);
    }
    (*string)[index] = '\0';
    (*string) = realloc((*string), sizeof(char)* index+1);
    if (symbol == EOF) return end;
    return ok;
}

status to_polish(char* string, char** polish){
    int length = strlen(string);
    *polish = (char*)malloc(sizeof(char) * length * 2);
    if (!(*polish)) return allocation_error;
    Stack* stack = NULL;
    int index = 0;
    int depth = 0;
    for (int i = 0; i < length;){
        if (isdigit(string[i])){
            while (isdigit(string[i])){
                (*polish)[index] = string[i];
                index++;
                i++;
            } 
            (*polish)[index] = ' ';
            index++;
        }
        if (get_priority(string[i]) == 5 && string[i] != '\0') return wrong_symbol;
        if (get_priority(string[i]) == 1){
            if (string[i] == '(') {
                if (push(&stack, string[i]) == allocation_error){
                    delete_stack(&stack);
                    return allocation_error;
                }
                depth++;
            }
            else {
                while (stack != NULL && stack->data != '('){
                    (*polish)[index] = pop(&stack);
                    index++;
                    (*polish)[index] = ' ';
                    index++;
                } 
                if (stack != NULL) pop(&stack);
                depth--;
            }
            i++;
        }
        else if (string[i] != '\0'){
            if (stack == NULL || (get_priority(stack->data) < get_priority(string[i]))){
                if (push(&stack, string[i]) == allocation_error){
                    delete_stack(&stack);
                    return allocation_error;
                }
            }
            else {
                while (stack != NULL && (get_priority(stack->data) >= get_priority(string[i]))){
                    (*polish)[index] = pop(&stack);
                    index++;
                    (*polish)[index] = ' ';
                    index++;
                }
                if (push(&stack, string[i]) == allocation_error){
                    delete_stack(&stack);
                    return allocation_error;
                }
            }
            i++;
        }
    }
    while (stack != NULL){
        (*polish)[index] = pop(&stack);
        index++;
        (*polish)[index] = ' ';
        index++;
    }
    (*polish)[index] = '\0';
    if (depth != 0) return wrong_balance;
    return ok;
}

int my_pow (int base, int power) {
    if (power == 0) return 1;
    if (power % 2 == 1) return my_pow(base, power - 1) * base;
    else {
        double result = my_pow(base, power / 2);
        return result * result;
    }
}

status calculate(const char* string, int* answer){
    int length = strlen(string);
    int number = 0;
    int number2, number1;
    char operand;
    Stack* stack = NULL;
    
    for (int i = 0; i < length; i++){
        if (isdigit(string[i])){
            number = number * 10 + (string[i] - '0');
        }
        else if (string[i] == ' ' && isdigit(string[i-1])){
            if (push(&stack, number) == allocation_error){
                        delete_stack(&stack);
                        return allocation_error;
                    }
            number = 0;
        }
        else if (string[i] == ' ') continue;
        else {
            number1 = pop(&stack);
            number2 = pop(&stack);
            switch(string[i]){
                case '-':
                    if (push(&stack, number2 - number1) == allocation_error){
                        delete_stack(&stack);
                        return allocation_error;
                    }
                    break;
                case '+':
                    if (push(&stack, number2 + number1) == allocation_error){
                        delete_stack(&stack);
                        return allocation_error;
                    }
                    break;
                case '/':
                    if (push(&stack, number2 / number1) == allocation_error){
                        delete_stack(&stack);
                        return allocation_error;
                    }
                    break;
                case '%':
                    if (push(&stack, number2 % number1) == allocation_error){
                        delete_stack(&stack);
                        return allocation_error;
                    }
                    break;
                case '*':
                    if (push(&stack, number2 * number1) == allocation_error){
                        delete_stack(&stack);
                        return allocation_error;
                    }
                    break;
                case '^':
                    if (number < 0) return wrong_power;
                    if (push(&stack, my_pow(number2, number1)) == allocation_error){
                        delete_stack(&stack);
                        return allocation_error;
                    }
                    break;
            }
        }
    }
    *answer = pop(&stack);
    return ok;
}

void error_file(char* filename){
    char* new;
    new = strtok(filename, ".");
    strcat(new ,"_errors.txt\0");
    return;
}

void print_error(FILE* file, status error){
    switch(error){
        case wrong_symbol:
            fprintf(file, "wrong symbol\n");
        case wrong_power:
            fprintf(file, "wrong power\n");
        case wrong_balance:
            fprintf(file, "wrong bracket balance\n");
    }
}

int main(int argc, char* argv[]){
    if (argc < 2) {
        printf("Wrong amount of arguments");
        return -1;
    }
    char* string = NULL;
    char* polish = NULL;
    int answer;
    char filename[BUFSIZ];
    status flag = ok;
    status answer_flag;
    status polish_flag;
    FILE* out = NULL;
    int strnumber;
    for (int filenumber = 1; filenumber < argc; filenumber++){
        FILE* file = fopen(argv[filenumber], "r");
        printf("%s\n", argv[filenumber]);
        if (!file) {
            printf("Wrong file");
            return -2;
        }
        flag = ok;
        strnumber = 0;
        while (flag != end){
            flag = scan_string(file, &string);
            if (strlen(string) == 0){
                strnumber++;
                continue;
            }
            if (flag == allocation_error){
                printf("Allocation error");
                return -3;
            }
            else {
                polish_flag = to_polish(string, &polish);
                if (polish_flag == allocation_error){
                    free(string);
                    printf("Allocation error");
                    return -3;
                }
                else if (polish_flag == wrong_symbol){
                    if (!out){
                        strcpy(filename, argv[filenumber]);
                        error_file(filename);
                        out = fopen(filename, "w");
                    }
                    fprintf(out, "in string %d detected wrong symbol\n", strnumber);
                }
                else if (polish_flag == wrong_balance){
                    if (!out){
                        strcpy(filename, argv[filenumber]);
                        error_file(filename);
                        out = fopen(filename, "w");
                    }
                    fprintf(out, "in string %d detected wrong bracket balance\n", strnumber);
                }
                else{
                    answer_flag = calculate(polish, &answer);
                    switch(answer_flag){
                        case allocation_error:
                            free(string);
                            free(polish);
                            printf("Allocation error");
                            return -3;
                        case wrong_power:
                            if (!out){
                                strcpy(filename, argv[filenumber]);
                                error_file(filename);
                                out = fopen(filename, "w");
                            }
                            fprintf(out, "in string %d detected wrong power\n", strnumber);
                        case ok:
                            printf("Expression: %s\n", string);
                            printf("In polish: %s\n", polish);
                            printf("Result: %d\n\n", answer);
                    }
                }
            }
            strnumber++;
            free(string);
            string = NULL;
            free(polish);
            polish = NULL;
        }
        fclose(file);
        file = NULL;
        fclose(out);
        out = NULL;
    }
    return 0;
}
