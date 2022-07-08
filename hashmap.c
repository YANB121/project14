#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include "hashmap.h"


unsigned long hash_function(char *input,int size) {
   unsigned long i;
    i = 0;
    for (int j = 0; input[j]; j++)
       i += input[j];
   return i % size;
 //  return 0l;
}




struct Ht_item {
    char *key;
    char *value;
};


typedef struct HashTable HashTable;
typedef struct LinkedList LinkedList;

struct HashTable {

    Ht_item **items;
    int size;
    int count;
    LinkedList **overflow_buckets;
};



Ht_item *create_item(char *key, char *value) {

    Ht_item *item = (Ht_item *) malloc(sizeof(Ht_item));
    item->key = (char *) malloc(strlen(key) + 1);
    item->value = (char *) malloc(strlen(value) + 1);

    strcpy(item->key, key);
    strcpy(item->value, value);

    return item;
}

void free_item(Ht_item *item) {
    // Frees an item
    free(item->key);
    free(item->value);
    free(item);
}

LinkedList **create_overflow_buckets(HashTable *table) {
    // Create the overflow buckets; an array of linkedlists
    LinkedList **buckets = (LinkedList **) calloc(table->size, sizeof(LinkedList *));
    for (int i = 0; i < table->size; i++)
        buckets[i] = NULL;
    return buckets;
}

void free_overflow_buckets(HashTable *table) {
    // Free all the overflow bucket lists
    LinkedList **buckets = table->overflow_buckets;
    for (int i = 0; i < table->size; i++)
        free_linkedlist(buckets[i]);
    free(buckets);
}

HashTable *create_table(int size) {
    // Creates a new HashTable
    HashTable *table = (HashTable *) malloc(sizeof(HashTable));
    table->size = size;
    table->count = 0;
    table->items = (Ht_item **) calloc(table->size, sizeof(Ht_item *));
    for (int i = 0; i < table->size; i++)
        table->items[i] = NULL;
    table->overflow_buckets = create_overflow_buckets(table);

    return table;
}

void free_table(HashTable *table) {
    // Frees the table
    for (int i = 0; i < table->size; i++) {
        Ht_item *item = table->items[i];
        if (item != NULL)
            free_item(item);
    }


    free_overflow_buckets(table);
    free(table->items);
    free(table);
}

typedef struct LinkedList LinkedList;


struct LinkedList {
    Ht_item *item;
    LinkedList *next;
};

LinkedList *allocate_list() {
    LinkedList *list = (LinkedList *) malloc(sizeof(LinkedList));
    return list;
}

LinkedList *linkedlist_insert(LinkedList *list, Ht_item *item) {
    if (!list) {
        LinkedList *head = allocate_list();
        head->item = item;
        head->next = NULL;
        list = head;
        return list;
    } else if (list->next == NULL) {
        LinkedList *node = allocate_list();
        node->item = item;
        node->next = NULL;
        list->next = node;
        return list;
    }

    LinkedList *temp = list;
    while (temp->next->next) {
        temp = temp->next;
    }

    LinkedList *node = allocate_list();
    node->item = item;
    node->next = NULL;
    temp->next = node;

    return list;
}

Ht_item *linkedlist_remove(LinkedList *list) {
    // Removes the head from the linked list
    // and returns the item of the popped element
    if (!list)
        return NULL;
    if (!list->next)
        return NULL;
    LinkedList *node = list->next;
    LinkedList *temp = list;
    temp->next = NULL;
    list = node;
    Ht_item *it = NULL;
    memcpy(temp->item, it, sizeof(Ht_item));
    free(temp->item->key);
    free(temp->item->value);
    free(temp->item);
    free(temp);
    return it;
}

void free_linkedlist(LinkedList *list) {
    LinkedList *temp = list;
    while (list) {
        temp = list;
        list = list->next;
        free(temp->item->key);
        free(temp->item->value);
        free(temp->item);
        free(temp);
    }
}

void handle_collision(HashTable *table, unsigned long index, Ht_item *item) {
    LinkedList *head = table->overflow_buckets[index];

    if (head == NULL) {
        // We need to create the list
        head = allocate_list();
        head->item = item;
        head->next = NULL;
        table->overflow_buckets[index] = head;
        return;
    } else {
        // Insert to the list
        table->overflow_buckets[index] = linkedlist_insert(head, item);
        return;
    }
}


void ht_insert(HashTable *table, char *key, char *value) {

    Ht_item *item = create_item(key, value);

    // Compute the index
    unsigned long index = hash_function(key,table->size);

    Ht_item *current_item = table->items[index];

    if (current_item == NULL) {

        if (table->count == table->size) {
            // Hash Table Full
            printf("Insert Error: Hash Table is full\n");
            // Remove the create item
            free_item(item);
            return;
        }

        // Insert directly
        table->items[index] = item;
        table->count++;
    } else {

        if (strcmp(current_item->key, key) == 0) {
            strcpy(table->items[index]->value, value);
            return;
        } else {

            handle_collision(table, index, item);
            return;
        }
    }
}

char *ht_search(HashTable *table, char *key) {

    int index = hash_function(key,table->size);
    Ht_item *item = table->items[index];


    if (item != NULL) {
        if (strcmp(item->key, key) == 0)
            return item->value;
        else {
            LinkedList *temp = table->overflow_buckets[index];
            if (temp != NULL) {
                do {
                    Ht_item *temp_item = temp->item;
                    if (strcmp(temp_item->key, key) == 0)
                        return temp_item->value;
                    temp = temp->next;
                } while (temp != NULL);

            }
            return NULL;
        }
    }
}


void print_search(HashTable *table, char *key) {
    char *val;
    if ((val = ht_search(table, key)) == NULL) {
        printf("Key:%s does not exist\n", key);
        return;
    } else {
        printf("Key:%s, Value:%s\n", key, val);
    }
}

void print_table(HashTable *table) {
    printf("\nHash Table\n-------------------\n");
    LinkedList **overflowed_list = table->overflow_buckets;

    for (int i = 0; i < table->size; i++) {
        if (table->items[i]) {
            printf("Index:%d, Key:%s, Value:%s\n", i, table->items[i]->key, table->items[i]->value);
        }
        if (overflowed_list[i]) {
            LinkedList *current = overflowed_list[i];
            do {
                if (current->item)
                    printf("Index:%d, Key:%s, Value:%s\n", i, current->item->key, current->item->value);
                current = current->next;
            } while (current);

        }
    }
    printf("-------------------\n\n");
}



