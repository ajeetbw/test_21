/**
 * @file inventory.h
 * @brief Defines the Inventory structure and management functions.
 */
#ifndef INVENTORY_H
#define INVENTORY_H

#include "item.h"

/**
 * @struct Inventory
 * @brief Manages a dynamic array of items.
 */
typedef struct {
    Item* items;      /**< Pointer to dynamically allocated array of items */
    int count;        /**< Current number of items in the inventory */
    int capacity;     /**< Maximum capacity before reallocation is needed */
} Inventory;

/**
 * @brief Initializes an empty inventory with a specified initial capacity.
 * 
 * @param inv Pointer to the inventory to initialize.
 * @param initial_capacity The starting capacity.
 */
void init_inventory(Inventory* inv, int initial_capacity);

/**
 * @brief Frees the dynamically allocated memory used by the inventory.
 * 
 * @param inv Pointer to the inventory to free.
 */
void free_inventory(Inventory* inv);

/**
 * @brief Adds an item to the inventory, expanding memory if necessary.
 * 
 * @param inv Pointer to the inventory.
 * @param item The item to add.
 * @return 1 on success, 0 on failure (e.g., out of memory).
 */
int add_item(Inventory* inv, Item item);

/**
 * @brief Removes an item from the inventory by its ID.
 * 
 * @param inv Pointer to the inventory.
 * @param id The ID of the item to remove.
 * @return 1 on success, 0 if the item was not found.
 */
int remove_item(Inventory* inv, int id);

/**
 * @brief Searches for an item by its ID.
 * 
 * @param inv Pointer to the inventory.
 * @param id The ID of the item to find.
 * @return Pointer to the found item, or NULL if not found.
 */
Item* find_item(const Inventory* inv, int id);

/**
 * @brief Lists all items currently in the inventory to standard output.
 * 
 * @param inv Pointer to the inventory.
 */
void list_items(const Inventory* inv);

#endif // INVENTORY_H