
#ifndef MAMN14_PREPROCESSOR_H
#define MAMN14_PREPROCESSOR_H

#include "hashmap.h"

FILE *create_macro_file(char *);

void span_macros(char *);

void fetch_write_macros(FILE *file, HashTable *hashTable, char *);

void write_macros(FILE *, HashTable);

void macro_file_path(char *);

#endif //MAMN14_PREPROCESSOR_H
