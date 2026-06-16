/**
 * @file inventory.c
 * @brief Implementation of inventory management functions.
 */
#include <stdio.h>
#include <stdlib.h>
#include "inventory.h"

void init_inventory(Inventory* inv, int initial_capacity) {
    inv->capacity = initial_capacity > 0 ? initial_capacity : 10;
    inv->count = 0;
    
    inv->items = (Item*)malloc(inv->capacity * sizeof(Item));
    if (inv->items == NULL) {
        fprintf(stderr, "Critical Error: Memory allocation failed for inventory.\n");
        exit(EXIT_FAILURE);
    }
}

void free_inventory(Inventory* inv) {
    if (inv->items != NULL) {
        free(inv->items);
        inv->items = NULL;
    }
    inv->count = 0;
    inv->capacity = 0;
}

int add_item(Inventory* inv, Item item) {
    // Check if we need to expand the dynamic array
    if (inv->count >= inv->capacity) {
        int new_capacity = inv->capacity * 2;
        Item* temp = (Item*)realloc(inv->items, new_capacity * sizeof(Item));
        
        if (temp == NULL) {
            fprintf(stderr, "Error: Memory reallocation failed during add_item.\n");
            return 0; // Failure
        }
        
        inv->items = temp;
        inv->capacity = new_capacity;
    }
    
    // Add the new item to the end of the array
    inv->items[inv->count++] = item;
    return 1; // Success
}

int remove_item(Inventory* inv, int id) {
    for (int i = 0; i < inv->count; i++) {
        if (inv->items[i].id == id) {
            // Shift elements left to fill the gap
            for (int j = i; j < inv->count - 1; j++) {
                inv->items[j] = inv->items[j + 1];
            }
            inv->count--;
            return 1; // Success
        }
    }
    return 0; // Not found
}

Item* find_item(const Inventory* inv, int id) {
    for (int i = 0; i < inv->count; i++) {
        if (inv->items[i].id == id) {
            return &inv->items[i];
        }
    }
    return NULL;
}

void list_items(const Inventory* inv) {
    if (inv->count == 0) {
        printf("The inventory is currently empty.\n");
        return;
    }
    
    printf("\n====================================================================\n");
    printf("                       CURRENT INVENTORY                            \n");
    printf("====================================================================\n");
    for (int i = 0; i < inv->count; i++) {
        print_item(&inv->items[i]);
    }
    printf("====================================================================\n");
    printf("Total distinct items: %d\n", inv->count);
}