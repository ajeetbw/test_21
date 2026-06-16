/**
 * @file item.c
 * @brief Implementation of item operations.
 */
#include <stdio.h>
#include <string.h>
#include "item.h"

Item create_item(int id, const char* name, int quantity, double price) {
    Item new_item;
    new_item.id = id;
    
    // Safely copy the string to prevent buffer overflows
    strncpy(new_item.name, name, MAX_NAME_LEN - 1);
    new_item.name[MAX_NAME_LEN - 1] = '\0'; // Ensure null-termination
    
    new_item.quantity = quantity;
    new_item.price = price;
    
    return new_item;
}

void print_item(const Item* item) {
    if (item != NULL) {
        printf("ID: %04d | Name: %-25s | Qty: %-5d | Price: $%.2f\n",
               item->id, item->name, item->quantity, item->price);
    }
}