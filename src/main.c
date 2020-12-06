#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "sortings.h"

#define error(...) (fprintf(stderr, __VA_ARGS__))

typedef void (*sort_alg_t)(strings_array_t, array_size_t, comparator_func_t);

struct input_argc_t{
    array_size_t num_strings;
    comparator_func_t comparator;
    sort_alg_t sort_alg;
};

int asc(const char* string_1, const char* string_2){
    return strcmp(string_1,string_2);
}

int des(const char* string_1, const char* string_2){
    return -strcmp(string_1,string_2);
}

int review_entering_args(int argc, char* argv[], struct input_argc_t* input_argc){
    if (argc != 6){
        error("There must be 5 arguments");
        return -1;
    }

    errno = 0;
    char* end_pointer = NULL;
    const char* n_pointer = argv[1];
    if (strtol(argv[1],&end_pointer,10) < 0 || n_pointer == end_pointer) {
        error("Incorrect first argument.");
        return -1;
    }
    else {input_argc->num_strings = strtoul(argv[1], NULL, 10);
    }

    if (strstr(argv[2], ".txt") == NULL){
        error("Incorrect input file. File must be .txt type");
        return -1;
    }

    if (strstr(argv[3], ".txt") == NULL){
        error("Incorrect output file. File must be .txt type");
        return -1;
    }

    if (strcmp(argv[4], "bubble") == 0 ){
        input_argc->sort_alg = bubble;
    }
    else if (strcmp(argv[4], "insertion") == 0 ){
        input_argc->sort_alg = insertion;
    }
    else if (strcmp(argv[4], "merge") == 0 ){
        input_argc->sort_alg = merge;
    }
    else if (strcmp(argv[4], "quick") == 0 ){
        input_argc->sort_alg = quick;
    }
    else if (strcmp(argv[4], "radix") == 0 ){
        input_argc->sort_alg = radix;
    }
    else {
        error("Your sorting is not supported");
        return -1;
    }

    if (strcmp(argv[5], "asc") == 0 ){
        input_argc->comparator = asc;
    }
    else if (strcmp(argv[5], "des") == 0 ){
        input_argc->comparator = des;
        }
    else {
        error("Your comparator is not supported");
        return -1;
    }
    return 0;
}


int reading_input_file(FILE* input_file, struct input_argc_t* option, strings_array_t strings_array){
    for (size_t i = 0; i < option->num_strings ; i++) {
        if (!feof(input_file)) {
            if (fgets(strings_array[i], MAX_INPUT_STRING_SIZE, input_file) == NULL) {
                error("Fail getting string from input file");
                return -1;
            }
        }
        else if (feof(input_file) && i < option->num_strings){
            error("There are less strings in file than you want to be sorted");
            return -1;
        }
    }
    return 0;
}


int main(int argc, char* argv[]) {
    struct input_argc_t input_argc;
    if (review_entering_args(argc, argv, &input_argc ) < 0){
        return -1;
    }

    strings_array_t strings_array = (char**)malloc(sizeof(char*) * input_argc.num_strings);
    if (strings_array != NULL) {
        for(size_t i = 0; i < input_argc.num_strings; i++) {
            strings_array[i] = (char*)malloc(sizeof(char) * MAX_INPUT_STRING_SIZE);
            if(strings_array[i] == NULL) {
                for (size_t j = 0; j < input_argc.num_strings; j++){
                    free(strings_array[j]);
                }
                free(strings_array);
                strings_array = NULL;
                error("Fail in allocation memory\n");
                return -1;
            }
        }
    }
    else if (strings_array == NULL){
        error("Fail in allocation memory\n");
        return -1;
    }

    FILE *input_file = fopen(argv[2], "r");
    if (input_file == NULL) {
        for(size_t i = 0; i <input_argc.num_strings; i++) //free strings array
            free(strings_array[i]);
        free(strings_array);
        strings_array = NULL;
        error("Fail in opening input file\n");
        return -1;
    }

    if (reading_input_file(input_file, &input_argc, strings_array) < 0) {
            fclose(input_file);
            for (size_t i = 0; i < input_argc.num_strings; i++) //free strings array
                free(strings_array[i]);
            free(strings_array);
            strings_array = NULL;
            error("Fail in reading file\n");
            return -1;
    }

    if (input_argc.num_strings > 0) {
        input_argc.sort_alg(strings_array, input_argc.num_strings, input_argc.comparator);
    }

    FILE* output_file = fopen(argv[3], "w");
    if (output_file == NULL){
        fclose(input_file);
        for(size_t i = 0; i < input_argc.num_strings; i++) //free strings array
            free(strings_array[i]);
        free(strings_array);
        strings_array = NULL;
        error("Fail in opening output file\n");
        return -1;
    }

//putting strings in file

if (input_argc.num_strings == 0){
    if (fputs("\n", output_file) == EOF){
        error("Fail in writing strings in output file");
        return -1;
    }
}else {for (size_t i = 0; i < input_argc.num_strings; i++){
    if (fputs(strings_array[i], output_file) != EOF){
        if(strcspn(strings_array[i], "\n") == strlen(strings_array[i])) {
            if (fputs("\n", output_file) == EOF) {
                error("Fail in writing strings in output file\n");
                return -1;
            }
        }
    }
    else {
        error("Fail in writing strings in output file");
        return -1;
    }
 }
}

    fclose(input_file);
    fclose(output_file);
    for(size_t i = 0; i <input_argc.num_strings; i++) //free strings array
        free(strings_array[i]);
    free(strings_array);
    strings_array = NULL;

    return 0;
}
