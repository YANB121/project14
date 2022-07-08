#ifndef MAMN14_HASHMAP_H
#define MAMN14_HASHMAP_H



#include <stdio.h>
#include <stdlib.h>


 struct Entry {
    struct Entry *next;
    char *key;
    char *value;
};
unsigned long hash_function(char *input,int size);
typedef struct HashTable HashTable;
typedef struct LinkedList LinkedList;
typedef struct Ht_item Ht_item;
void free_linkedlist(LinkedList *pList);
Ht_item *create_item(char *key, char *value);
LinkedList **create_overflow_buckets(HashTable *table);
void free_overflow_buckets(HashTable *table);
HashTable *create_table(int size);
void free_table(HashTable *table);
LinkedList *allocate_list();
LinkedList *linkedlist_insert(LinkedList *list, Ht_item *item);
Ht_item *linkedlist_remove(LinkedList *list);
void free_linkedlist(LinkedList *list);
void handle_collision(HashTable *table, unsigned long index, Ht_item *item);
void ht_insert(HashTable *table, char *key, char *value);
char *ht_search(HashTable *table, char *key);
void print_search(HashTable *table, char *key);
void print_table(HashTable *table);


#endif //MAMN14_HASHMAP_H
