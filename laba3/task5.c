#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


typedef struct{
    unsigned int id;
    char* name;
    char* surname;
    char* group;
    unsigned char* marks;
} Student;

typedef enum{
    ok,
    allocation_error,
    wrong_data,
    end
} status;

status id_validation(const char* id_string);
status name_validation(const char* string);
status group_validation(const char* string);
status marks_validation(const char* string);
status scan_string(FILE* file, char** string);
void skip_delimiter(FILE* file, char* a);
status make_array(FILE* file, Student** array, int* students_amount, double* average);
void print_array(FILE* file, const Student* array, const int students_amount);
int id_comparator(const void* student1, const void* student2);
int name_comparator(const void* student1, const void* student2);
int surname_comparator(const void* student1, const void* student2);
int group_comparator(const void* student1, const void* student2);
void search_id(FILE* out, const Student* array, const int students_amount, int id);
void search_name(FILE* out, const Student* array, const int students_amount, char* name);
void search_surname(FILE* out, const Student* array, const int students_amount, char* surname);
void search_group(FILE* out, const Student* array, const int students_amount, char* group);
void find_better_avg(FILE* out, const Student* array, const int students_amount, double average);
void print_menu();
status get_command(int* command);
void delete(Student* array, FILE* in, FILE* out);
status get_string_from_terminal(char** string);


int main(int argc, char* argv[]){
    if (argc != 3){
        printf("Wrong amount of arguments");
        return -1;
    }
    FILE* in = fopen(argv[1], "r");
    if (!in) {
        printf("Wrong filename");
        return -2;
    }
    FILE* out = fopen(argv[2], "w");
    if (!out){
        fclose(in);
        printf("Wrong filename");
        return -2;
    }
    Student* array = NULL;
    int students_amount = 0;
    double average = 0;
    status current;
    current = make_array(in, &array, &students_amount, &average);
    if (current == wrong_data || current == allocation_error){
        for (int i = 0; i < students_amount; i++){
            free(array[i].group);
            free(array[i].marks);
            free(array[i].name);
            free(array[i].surname);
        }
        free(array);
        fclose(in);
        fclose(out);
        if (current == wrong_data){
            printf("Wrong data detected");
            return -3;
        }
        printf("Allocation error");
        return -4;
    }
    int command = 0;
    char* string = NULL;
    while(command != 11 && command != -1){
        free(string);
        string = NULL;
        print_menu();
        if (command == -1) {
            printf("Allocation error\n");
            break;
        }
        if (get_command(&command) == wrong_data) {
            printf("Wrong command\n");
            command = 0;
        }
        switch(command){
            case 1:
                qsort(array, students_amount, sizeof(Student), (int(*) (const void*, const void*))id_comparator);
                printf("Done\n");
                break;
            case 2:
                qsort(array, students_amount, sizeof(Student), (int(*) (const void*, const void*))name_comparator);
                printf("Done\n");
                break;
            case 3:
                qsort(array, students_amount, sizeof(Student), (int(*) (const void*, const void*))surname_comparator);
                printf("Done\n");
                break;
            case 4:
                qsort(array, students_amount, sizeof(Student), (int(*) (const void*, const void*))group_comparator);
                printf("Done\n");
                break;
            case 5:
                printf("Enter id: ");
                if (get_string_from_terminal(&string) == allocation_error){
                    command = -1;
                    break;
                }
                if (id_validation(string) != ok) {
                    printf("Wrong id\n");
                    break;
                }
                search_id(out, array, students_amount, atoi(string));
                printf("Done\n");
                fprintf(out, "\n");
                break;
            case 6:
                printf("Enter name: ");
                if (get_string_from_terminal(&string) == allocation_error){
                    command = -1;
                    break;
                }
                if (name_validation(string) != ok) {
                    printf("Wrong id\n");
                    break;
                }
                search_name(out, array, students_amount, string);
                printf("Done\n");
                fprintf(out, "\n");
                break;
            case 7:
                printf("Enter surname: ");
                if (get_string_from_terminal(&string) == allocation_error){
                    command = -1;
                    break;
                }
                if (name_validation(string) != ok) {
                    printf("Wrong id\n");
                    break;
                }
                search_surname(out, array, students_amount, string);
                printf("Done\n");
                fprintf(out, "\n");
                break;
            case 8:
                printf("Enter group: ");
                if (get_string_from_terminal(&string) == allocation_error){
                    command = -1;
                    break;
                }
                search_surname(out, array, students_amount, string);
                printf("Done\n");
                fprintf(out, "\n");
                break;
            case 9:
                find_better_avg(out, array, students_amount, average);
                printf("Done\n");
                fprintf(out, "\n");
                break;
            case 10:
                print_array(out, array, students_amount);
                printf("Done\n");
                break;
            case 11:
                printf("Bye-bye");
                break;
            default:
                printf("Wrong command\n");
                break;
        }
    }
    for (int i = 0; i < students_amount; i++){
        free(array[i].group);
        free(array[i].marks);
        free(array[i].name);
        free(array[i].surname);
    }
    free(array);
    fclose(in);
    fclose(out);
    if (command == -1) {
        printf("Allocation error");
        return -4;
    }
    return 0;
}

status scan_string(FILE* file, char** string){
    int capacity = 2;
    *string = (char*)malloc(sizeof(char) * capacity);
    if (!(*string)) return allocation_error;
    char symbol = fgetc(file);
    skip_delimiter(file, &symbol);
    int index = 0;
    while (symbol != ' ' && symbol != '\n' && symbol != '\t' && symbol != EOF){
        (*string)[index] = symbol;
        index++;
        if (index+1 == capacity){
            capacity *= 2;
            char* ptr = realloc((*string), sizeof(char) * capacity);
            if (!ptr){
                free(*string);
                return allocation_error;
            }
            *string = ptr;
        }
        symbol = fgetc(file);
    }
    (*string)[index] = '\0';
    (*string) = realloc((*string), sizeof(char)*(index+1));
    if (symbol == EOF) return end;
}

void skip_delimiter(FILE* file, char* a){
    while ((*a) == ' ' || (*a) == '\n' || (*a) == '\t') *a = fgetc(file);
}

status id_validation(const char* id_string){
    int length = strlen(id_string);
    for (int i = 0; i < length; i++){
        if (!isdigit(id_string[i])) return wrong_data;
    }
    return ok;
}

status name_validation(const char* string){
    int length = strlen(string);
    for (int i = 0; i < length; i++){
        if (!isalpha(string[i])) return wrong_data;
    }
    return ok;
}

status group_validation(const char* string){
    if (strlen(string) != 0) return ok;
    return wrong_data;
}

status marks_validation(const char* string){
    if (strlen(string) != 5) return wrong_data;
    for (int i = 0; i < 5; i++){
        if (string[i] < '1' || string[i] > '5') return wrong_data;
    }
    return ok;
}

status make_array(FILE* file, Student** array, int* students_amount, double* average){
    int capacity = 2;
    *array = malloc(sizeof(Student)* capacity);
    if (!(*array)){
        return allocation_error;
    }
    char* id = NULL;
    char* name = NULL;
    char* surname = NULL;
    char* group = NULL;
    char* marks = NULL;
    status current = ok;
    while (current != end){
        Student* student = (Student*)malloc(sizeof(Student));
        if (!student) return allocation_error;
        current = scan_string(file, &id);
        if (current == allocation_error){
            return allocation_error;
        }
        if (id == NULL) return end;
        if (id_validation(id) != ok){
            return wrong_data;
        }
        student->id = atoi(id);
        current = scan_string(file, &name);
        if (current == allocation_error){
            return allocation_error;
        }
        if (current == end) return wrong_data;
        if (name_validation(name) != ok){
            return wrong_data;
        }
        student->name = name;
        current = scan_string(file, &surname);
        if (current == allocation_error){
            return allocation_error;
        }
        if (current == end) return wrong_data;
        if (name_validation(surname) != ok){
            return wrong_data;
        }
        student->surname = surname;
        current = scan_string(file, &group);
        if (current == allocation_error){
            return allocation_error;
        }
        if (current == end) return wrong_data;
        student->group = group;
        current = scan_string(file, &marks);
        if (current == allocation_error){
            return allocation_error;
        }
        if (marks_validation(marks) != ok){
            return wrong_data;
        }
        for (int i = 0; i < 5; i++){
            marks[i] = marks[i] - '0';
            *average += marks[i];
        }
        student->marks = marks;
        (*array)[*students_amount] = *student;
        id = NULL;
        name = NULL;
        surname = NULL;
        group = NULL;
        marks = NULL;
        (*students_amount)++;
        if (*students_amount == capacity){
            capacity *= 2;
            Student* ptr = realloc(*array, sizeof(Student) * capacity);
            if (!ptr) return allocation_error;
            *array = ptr;
        }
    }
    *average /= 5 * (*students_amount);
    return ok;
}

void print_array(FILE* out, const Student* array, const int students_amount){
    fprintf(out, "id name surname group   marks\n");
    for (int i = 0; i < students_amount; i++){
        fprintf(out, "%d\t%s\t%s\t%s\t%c %c %c %c %c\n", array[i].id, array[i].name, array[i].surname, array[i].group, array[i].marks[0]+'0', array[i].marks[1]+'0', array[i].marks[2]+'0', array[i].marks[3]+'0', array[i].marks[4]+'0');
    }
    fprintf(out, "\n");
}

int id_comparator(const void* student1, const void* student2){
    const Student* a = (const Student*)student1;
    const Student* b = (const Student*)student2;
    return a->id > b->id;
}

int name_comparator(const void* student1, const void* student2){
    const Student* a = (const Student*)student1;
    const Student* b = (const Student*)student2;
    return strcmp(a->name, b->name);
}

int surname_comparator(const void* student1, const void* student2){
    const Student* a = (const Student*)student1;
    const Student* b = (const Student*)student2;
    return strcmp(a->surname, b->surname);
}

int group_comparator(const void* student1, const void* student2){
    const Student* a = (const Student*)student1;
    const Student* b = (const Student*)student2;
    return strcmp(a->group, b->group);
}

void search_id(FILE* out, const Student* array, const int students_amount, int id){
    for (int i = 0; i < students_amount; i++){
        if (array[i].id == id) {
            double average = 0;
            for (int j = 0; j < 5; j++){
                average += array[i].marks[j];
            }
            fprintf(out, "%s %s %s %f\n", array[i].name, array[i].surname, array[i].group, average/5);
        }
    }
}

void search_name(FILE* out, const Student* array, const int students_amount, char* name){
    for (int i = 0; i < students_amount; i++){
        if (strcmp(array[i].name, name) == 0) {
            double average = 0;
            for (int j = 0; j < 5; j++){
                average += array[i].marks[j];
            }
            fprintf(out, "%s %s %s %f\n", array[i].name, array[i].surname, array[i].group, average/5);
        }
    }
}

void search_surname(FILE* out, const Student* array, const int students_amount, char* surname){
    for (int i = 0; i < students_amount; i++){
        if (strcmp(array[i].surname, surname) == 0) {
            double average = 0;
            for (int j = 0; j < 5; j++){
                average += array[i].marks[j];
            }
            fprintf(out, "%s %s %s %f\n", array[i].name, array[i].surname, array[i].group, average/5);
        }
    }
}

void search_group(FILE* out, const Student* array, const int students_amount, char* group){
    for (int i = 0; i < students_amount; i++){
        if (strcmp(array[i].group, group) == 0) {
            double average = 0;
            for (int j = 0; j < 5; j++){
                average += array[i].marks[j];
            }
            fprintf(out, "%s %s %s %f\n", array[i].name, array[i].surname, array[i].group, average/5);
        }
    }
}

void find_better_avg(FILE* out, const Student* array, const int students_amount, double average){
    double epsilon = 0.00000001;
    double current_average = 0;
    for (int i = 0; i < students_amount; i++){
        for (int j = 0; j < 5; j++){
            current_average += array[i].marks[j];
        }
        current_average /= 5;
        if (current_average - average > epsilon) {
            fprintf(out, "%s %s %s %f\n", array[i].name, array[i].surname, array[i].group, current_average);
        }
        current_average = 0;
    }
}

void print_menu(){
    printf("Choose command:\n");
    printf("1) sort by id\n");
    printf("2) sort by name\n");
    printf("3) sort by surname\n");
    printf("4) sort by group\n");
    printf("5) find by id\n");
    printf("6) find by name\n");
    printf("7) find by surname\n");
    printf("8) find by group\n");
    printf("9) find students with average mark more than average\n");
    printf("10) print array to file\n");
    printf("11) exit\n");
}

status get_command(int* command){
    char a = getchar();
    *command = 0;
    while (a == ' ' || a == '\n' || a == '\t' || a == '\n') a = getchar();
    while (!(a == ' ' || a == '\n' || a == '\t' || a == '\n')) {
        if (!isdigit(a)) {
            *command = 0;
            break;
        }
        *command = (*command) * 10 + (a - '0');
        a = getchar();
    }
    while (a != '\n') a = getchar();
    if (*command == 0) return wrong_data;
    if (*command > 11) return wrong_data;
    return ok;
}

status get_string_from_terminal(char** string){
    int capacity = 2;
    *string = (char*) malloc(sizeof(char) * capacity);
    if (!(*string)) return allocation_error;
    char a = getchar();
    int i = 0;
    while (a == ' ' || a == '\n' || a == '\t' || a == '\n') a = getchar();
    while (!(a == ' ' || a == '\n' || a == '\t' || a == '\n')) {
        (*string)[i] = a;
        i++;
        if (i+1 == capacity) {
            capacity *= 2;
            char* ptr = realloc(*string, sizeof(char) * capacity);
            if (!ptr){
                free(*string);
                return allocation_error;
            }
            (*string) = ptr;
        }
        a = getchar();
    }
    while (a != '\n') a = getchar();
    (*string)[i] = '\0';
    return ok;
}
