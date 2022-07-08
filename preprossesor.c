#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "preprocessor.h"
#include "hashmap.h"

#define ENDMACRO "endmacro"
#define MACRO "macro"
#define SIZE 30


void fetch_write_macros(FILE *file, HashTable *hashTabl, char *);

FILE *create_macro_file(char *);

void write_macros(FILE *file, HashTable macros);

void macro_file_path(char *path);


void span_macros(char *path) {

    FILE *file = fopen(path, "r");

    if (!file) { //test if the input file is valid
        printf("input file not found");
        exit(1);
    }


    HashTable *hashTable = create_table(SIZE);
    fetch_write_macros(file, hashTable, path); //fetch all macros from the asm file and insert them into the hashtable
}


void fetch_write_macros(FILE *file, HashTable *hashTable, char *path) {
    int bufferLength = 255;
    char buffer[255];

    char delimiter[] = " ";
    char *temp_copy;
    char *first_word;
    char *second_word;
    char *reminder;
    char *macro = calloc(1, 80);
    char *temp_macro;
    char *key;
    size_t macro_size = 0;
    int in_macro = 0;

    FILE *am_file = create_macro_file(path);


    while (fgets(buffer, bufferLength, file)) {

        temp_copy = calloc(1, 255);//temporary allocate buffer for string copy
        strcpy(temp_copy, buffer); //copy the original line into temporary line

        first_word = strtok_r(temp_copy, delimiter, &reminder); //fetch the first word from the line
        first_word[strcspn(first_word, "\n")] = 0; //trim the \n
        second_word = strtok_r(NULL, delimiter, &reminder); //fetch the second word from line

        if (second_word != NULL)
            second_word[strcspn(second_word, "\n")] = 0; //trim the \n

        void *exist_macro = ht_search(hashTable, first_word);

        if (!strcmp(first_word, ENDMACRO)) {
            in_macro = 0;
            ht_insert(hashTable, key, macro);
            macro_size = 0;
            macro = calloc(1, 80);
        } else if (in_macro == 1) {
            macro_size = macro_size + 1;
            temp_macro = calloc(macro_size, 80);
            strcpy(temp_macro, macro);
            free(macro);
            macro = temp_macro;
            strcat(macro, buffer);
        } else if (!strcmp(first_word, MACRO)) {
            in_macro = 1;
            key = calloc(1, 80);
            strcpy(key, second_word);
        } else if (exist_macro != NULL) {
            fputs(exist_macro, am_file);

        } else {
            fputs(buffer, am_file);
//            fputs("\n", am_file);
        }
        free(temp_copy);

    }

    fclose(file);
    fclose(am_file);
    print_table(hashTable);
}

FILE *create_macro_file(char *path2) {
    const char a[3] = ".am";
    path2[strlen(path2) - 3] = '\0'; //cut the last 3 characters from the string (the file extension)
    strncat(path2, a, 3); //add .am extension to the file name

    FILE *f;
    f = fopen(path2, "w");
    if (f == NULL)
        printf("Unable to create file.\n");

    return f;
}

void macro_file_path(char *path) {
    char *temp = calloc(1, sizeof(path));
    temp = strrev(temp);

    char *reminder;
}
