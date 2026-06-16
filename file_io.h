/**
 * @file file_io.h
 * @brief Handles reading from and writing to the persistent database file.
 */
#ifndef FILE_IO_H
#define FILE_IO_H

#include "inventory.h"

/**
 * @brief Saves the current inventory to a text-based database file.
 * 
 * @param inv Pointer to the inventory to save.
 * @param filename Path to the file.
 * @return 1 on success, 0 on failure.
 */
int save_inventory_to_file(const Inventory* inv, const char* filename);

/**
 * @brief Loads the inventory from a text-based database file.
 * 
 * @param inv Pointer to the inventory to populate.
 * @param filename Path to the file.
 * @return 1 on success, 0 if the file doesn't exist or is corrupted.
 */
int load_inventory_from_file(Inventory* inv, const char* filename);

#endif // FILE_IO_H