/**
 * @file item.h
 * @brief Defines the Item structure and related operations.
 */
#ifndef ITEM_H
#define ITEM_H

#define MAX_NAME_LEN 100

/**
 * @struct Item
 * @brief Represents a single product in the inventory.
 */
typedef struct {
    int id;                       /**< Unique identifier for the item */
    char name[MAX_NAME_LEN];      /**< Name of the item */
    int quantity;                 /**< Current stock quantity */
    double price;                 /**< Unit price of the item */
} Item;

/**
 * @brief Creates a new Item structure.
 * 
 * @param id The unique ID.
 * @param name The name of the item.
 * @param quantity The amount in stock.
 * @param price The cost per unit.
 * @return A populated Item structure.
 */
Item create_item(int id, const char* name, int quantity, double price);

/**
 * @brief Prints an item's details to the standard output.
 * 
 * @param item Pointer to the item to be printed.
 */
void print_item(const Item* item);

#endif // ITEM_H