#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

typedef struct Tree{
    int value;
    struct Tree* left;
    struct Tree* right;
} Tree;

typedef struct Stack {
    void* data;
    struct Stack* next;
} Stack;

typedef enum {
    ok,
    allocation_error,
    wrong_string,
    wrong_balance
} status;

int get_priority(const char a){
    if (a == '~') return 4;
    if (a == '?' || a == '!' || a == '+' || a == '&') return 3;
    if (a == '|' || a == '-' || a == '<' || a == '=') return 2;
    if (a == '(' || a == ')') return 1;
    return 0;
}



status push(Stack** stack, void* value){
    Stack* new = (Stack*)malloc(sizeof(Stack));
    if (!new) return allocation_error;
    new->data = value;
    new->next = *stack;
    *stack = new;
    return ok;
}

void* pop(Stack** stack){
    Stack* out;
    out = *stack;
    *stack = (*stack)->next;
    void* value = out->data;
    free(out);
    return value;
}

void delete_stack(Stack** stack){
    Stack* out;
    while (*stack != NULL){
        out = *stack;
        *stack = (*stack)->next;
        void* value = out->data;
        free(out);
    }
}

int is_operand(const char symbol){
    if (symbol == '~') return 3;
    if (symbol == '!' || symbol == '?' || symbol == '+' || symbol == '&') return 2;
    if (symbol == '|' || symbol == '-' || symbol == '<' || symbol == '=') return 1;
    return 0;
}

int is_delimiter(const char symbol){
    if (symbol == ' ' || symbol == '\t') return 1;
    if (symbol == '\n') return -1;
    return 0;
}

status scan_string(FILE* file, char** string){
    int capacity = 2;
    int index = 0;
    *string = (char*)malloc(sizeof(char) * capacity);
    if (!(*string)) return allocation_error;
    char a = fgetc(file);
    while (a != EOF){
        switch(is_delimiter(a)){
            case 1:
                break;
            case -1:
                free(*string);
                return wrong_string;
            case 0:
                (*string)[index] = a;
                index++;
                if (index + 1 == capacity){
                    capacity *= 2;
                    char* ptr = (char*)realloc(*string, sizeof(char) * capacity);
                    if (!ptr){
                        free(*string);
                        return wrong_string;
                    }
                    *string = ptr;
                }
                break;
        }
        a = fgetc(file);
    }
    (*string)[index] = '\0';
    char* ptr = (char*)realloc(*string, sizeof(char) * (index+1));
    if (!ptr) {
        free(string);
        return allocation_error;
    }
    *string = ptr;
    return ok;
}

status make_tree(Tree** tree, int value){
    *tree = (Tree*)malloc(sizeof(Tree));
    if (!tree) return allocation_error;
    (*tree)->value = value;
    (*tree)->left = NULL;
    (*tree)->right = NULL;
    return ok;
}

void delete_tree(Tree** tree){
    if (!(*tree)) return;
    delete_tree(&(*tree)->left);
    delete_tree(&(*tree)->right);
    free(*tree);
}

status to_postfix(char* string, char** postfix){
    int length = strlen(string);
    *postfix = (char*)malloc(sizeof(char) * length * 2);
    if (!(*postfix)) return allocation_error;
    Stack* stack = NULL;
    int index = 0;
    int depth = 0;
    for (int i = 0; i < length;){
        if (isdigit(string[i])){
            if (isalnum(string[i+1]) ||(string[i] != '0' && string[i] != '1')){
                free(*postfix);
                delete_stack(&stack);
                return wrong_string;
            }
            (*postfix)[index] = string[i];
            index++;
            i++;
            (*postfix)[index] = ' ';
            index++;
        }
        if (isalpha(string[i])){
            if (isalnum(string[i+1])){
                free(*postfix);
                delete_stack(&stack);
                return wrong_string;
            }
            (*postfix)[index] = string[i];
            index++;
            i++;
            (*postfix)[index] = ' ';
            index++;
        }
        if (!get_priority(string[i]) && string[i] != '\0') return wrong_string;
        if (get_priority(string[i]) == 1){
            if (string[i] == '(') {
                if (push(&stack, &string[i]) == allocation_error){
                    delete_stack(&stack);
                    return allocation_error;
                }
                depth++;
            }
            else {
                while (stack != NULL && *(char*)stack->data != '('){
                    (*postfix)[index] = *(char*)pop(&stack);
                    index++;
                    (*postfix)[index] = ' ';
                    index++;
                } 
                if (stack != NULL) pop(&stack);
                depth--;
            }
            i++;
        }
        else if (string[i] != '\0'){
            if (is_operand(string[i+1]) && string[i+1] != ')' && string[i+1] != '~'){
                delete_stack(&stack);
                return wrong_string;
            }
            if (stack == NULL || (get_priority(*(char*)stack->data) < get_priority(string[i]))){
                if (push(&stack, &string[i]) == allocation_error){
                    delete_stack(&stack);
                    return allocation_error;
                }
            }
            else {
                while (stack != NULL && (get_priority(*(char*)stack->data) >= get_priority(string[i]))){
                    (*postfix)[index] = *(char*)pop(&stack);
                    index++;
                    (*postfix)[index] = ' ';
                    index++;
                }
                if (push(&stack, &string[i]) == allocation_error){
                    delete_stack(&stack);
                    return allocation_error;
                }
            }
            if (string[i] == '+' || string[i] == '-' || string[i] == '<'){
                if (string[i+1] != '>') {
                    delete_stack(&stack);
                    return wrong_string;
                }
                i++;
            }
            i++;
        }
    }
    while (stack != NULL){
        (*postfix)[index] = *(char*)pop(&stack);
        index++;
        (*postfix)[index] = ' ';
        index++;
    }
    (*postfix)[index] = '\0';
    if (depth != 0) return wrong_string;
    return ok;
}

status postfix_to_tree(const char* postfix, Tree** tree, char** variables, int* variables_amount){
    Stack* stack = NULL;
    int length = strlen(postfix);
    Tree* new = NULL;
    int variables_capacity = 2;
    *variables = (char*)malloc(sizeof(char)* variables_capacity);
    int variable_amount = 0;
    bool flag = 1;
    for (int i = 0; i < length; i++){
        if (make_tree(&new, postfix[i]) == allocation_error) return allocation_error;
        if (is_operand(postfix[i])){
            if (postfix[i] == '~'){
                new->left = (Tree*)pop(&stack);
                if (push(&stack, new) == allocation_error) return allocation_error;
            }
            else{
                new->right = (Tree*)pop(&stack);
                new->left = (Tree*)pop(&stack);
                if (push(&stack, new) == allocation_error) return allocation_error;
            }
        }
        else{
            if (isalpha(postfix[i])){
                for(int j = 0; j < variable_amount; j++){
                    if (postfix[i] == (*variables)[j]) {
                        flag = 0;
                        break;
                    }
                }
                if (flag){
                    if (variable_amount+1 == variables_capacity){
                        variables_capacity *= 2;
                        char* ptr = (char*)realloc(*variables, sizeof(char) * variables_capacity);
                        if (!ptr) {
                            free(*variables);
                            return allocation_error;
                        }
                        *variables = ptr;
                    }
                    (*variables)[variable_amount] = postfix[i];
                    variable_amount++;
                }
            }
            if (push(&stack, new) == allocation_error) return allocation_error;
        }
        new = NULL;
        i++;
    }
    (*variables)[variable_amount] = '\0';
    *tree = (Tree*)pop(&stack);
    *variables_amount = variable_amount;
    return ok;
}

void print_file(FILE *file, const int *values, const int values_count, const int result) {
    for (int i = 0; i < values_count; i++) {
        fprintf(file, "%d | ", values[i]);
    }
    fprintf(file, " %d\n", result);
}

int calculation(const int a, const int b, const char operation){
    switch(operation){
        case '&':
            return a & b;
        case '|':
            return a | b;
        case '~':
            return ~b;
        case '-':
            return ~a | b;
        case '+':
            return ~(~a | b);
        case '<':
            return a^b;
        case '=':
            return a==b;
        case '!':
            return ~(a & b);
        case '?':
            return ~(a | b);
    }
}

status calculate_tree(const Tree* tree, int* vector, const char* variables, const int variables_amount, Stack** stack, int* result){
    if (is_operand(tree->value)){
        calculate_tree(tree->left, vector, variables, variables_amount, stack, result);
        if (tree->value != '~')
            calculate_tree(tree->right, vector, variables, variables_amount, stack, result);
    }
    if (isdigit(tree->value)){
        int* number = (int*)malloc(sizeof(int));
        if (!number) return allocation_error;
        if (tree->value == '0') *number = 0;
        else *number = 1;
        if (push(stack, number) == allocation_error){
            delete_stack(stack);
            return allocation_error;
        }
        return ok;
    }
    if (isalpha(tree->value)){
        int i = 0;
        for (; i < variables_amount; i++){
            if (tree->value == variables[i]){
                break;
            }
        }
        if (push(stack, &(vector[i])) == allocation_error){
            delete_stack(stack);
            return allocation_error;
        }
        return ok;
    }
    int* answer = (int*)malloc(sizeof(int));
    int a = *(int*)pop(stack);
    int b = 0;
    if (tree->value != '~')
        b = *(int*)pop(stack);
    else b = a;
    *answer = calculation(b, a, tree->value);
    if (tree->value == '~'){
        if (push(stack, &b) == allocation_error){
            delete_stack(stack);
            return allocation_error;
        }
    }
    if (push(stack, answer) == allocation_error){
        delete_stack(stack);
        return allocation_error;
    }
    *result = *(int*)(*stack)->data;
    return ok;
}

status enumeration(const Tree* tree, const int variables_amount, const char* variables, FILE* file){
    int vector[variables_amount];
    for (int i = 0; i < variables_amount; i++){
        vector[i] = 0;
    }
    int result;
    int index = 0;
    while (1) {
        Stack* stack = NULL;
        if (calculate_tree(tree, vector, variables, variables_amount, &stack, &result) == allocation_error){
            delete_stack(&stack);
            return allocation_error;
        }
        delete_stack(&stack);
        print_file(file, vector, variables_amount, result);
        int lastbit = ++index ^ (index - 1);
        int power = 0;
        while (lastbit >>= 1) power++;
        if (power < variables_amount)
            vector[power] = !vector[power];
        else break;
    }
}

void gener_filename(char *filename) {
    int size = rand() % 10 + 1;
    char result[size];
    for (int i = 0; i < size;) {
        result[i] = rand() % ('Z' - '0' + 1) + '0';
        if (isalpha(result[i]) || isdigit(result[i])) {
            i++;
        }
    }
    result[size] = '\0';
    strcat(result, ".txt");
    strcpy(filename, result);
}

void delete(FILE* in, FILE* out, char* string, char* postfix, char* variables, Tree* tree){
    fclose(in);
    fclose(out);
    free(string);
    free(postfix);
    free(variables);
    delete_tree(&tree);
}

int main(int argc, char* argv[]){
    if (argc != 2){
        printf("Wrong arguments amount");
        return -1;
    }
    FILE* in = fopen(argv[1], "r");
    if (!in){
        printf("File did not open");
        return -2;
    }
    char* filename = (char*)malloc(sizeof(char)* 15);
    gener_filename(filename);
    FILE* file = fopen(filename, "w");
    if (!file){
        fclose(in);
        printf("File did not open");
        return -2;
    }
    char* string = NULL;
    char* postfix = NULL;
    Tree* tree = NULL;
    int variable_amount = 0;
    char* variables = NULL;
    status flag = scan_string(in, &string);
    if (flag == allocation_error){
        printf("Allocation error");
        delete(in, file, string, postfix, variables, tree);
        return allocation_error;
    }
    if (flag == wrong_string){
        printf("Wrong string");
        delete(in, file, string, postfix, variables, tree);
        return wrong_string;
    }
    flag = to_postfix(string, &postfix);
    if (flag == allocation_error){
        printf("Allocation error");
        delete(in, file, string, postfix, variables, tree);
        return allocation_error;
    }
    if (flag == wrong_string){
        printf("Wrong string");
        delete(in, file, string, postfix, variables, tree);
        return wrong_string;
    }
    flag = postfix_to_tree(postfix, &tree, &variables, &variable_amount);
    if (flag == allocation_error){
        printf("Allocation error");
        delete(in, file, string, postfix, variables, tree);
        return allocation_error;
    }
    for (int i = 0; i < variable_amount; i++){
        fprintf(file, "%c | ", variables[i]);
    }
    fprintf(file, "%s\n", string);
    flag = enumeration(tree, variable_amount, variables, file);
    if (flag == allocation_error){
        printf("Allocation error");
        delete(in, file, string, postfix, variables, tree);
        return allocation_error;
    }
    delete(in, file, string, postfix, variables, tree);
    return 0;
    //printf("\n%s", postfix);
    //print_tree(tree, 0);
}