/**
 * @file main.c
 * @brief Application entry point. Ties together the inventory CLI.
 */
#include <stdio.h>
#include <stdlib.h>
#include "inventory.h"
#include "file_io.h"
#include "utils.h"

#define DATA_FILE "inventory_data.txt"

/**
 * @brief Displays the main application menu.
 */
void display_menu() {
    printf("\n--- Advanced Inventory Management System ---\n");
    printf("1. Add a New Item\n");
    printf("2. Remove an Item\n");
    printf("3. Search for an Item\n");
    printf("4. List All Items\n");
    printf("5. Save and Exit\n");
    printf("--------------------------------------------\n");
}

int main() {
    Inventory inv;
    
    // Initialize the dynamic inventory with space for 5 items to start
    init_inventory(&inv, 5);

    printf("Booting system... Loading database...\n");
    if (load_inventory_from_file(&inv, DATA_FILE)) {
        printf("Successfully loaded %d items from the database.\n", inv.count);
    } else {
        printf("No existing database found. Starting fresh.\n");
    }

    int running = 1;
    while (running) {
        display_menu();
        int choice = read_integer("Enter your choice (1-5): ");

        switch (choice) {
            case 1: {
                int id = read_integer("Enter unique item ID: ");
                if (find_item(&inv, id) != NULL) {
                    printf("Error: Item with ID %d already exists in the system.\n", id);
                    break;
                }
                
                char name[MAX_NAME_LEN];
                read_string("Enter item name: ", name, sizeof(name));
                int qty = read_integer("Enter initial quantity: ");
                double price = read_double("Enter price per unit: ");

                Item new_item = create_item(id, name, qty, price);
                if (add_item(&inv, new_item)) {
                    printf(">> Item '%s' successfully added to the system.\n", name);
                }
                break;
            }
            case 2: {
                int id = read_integer("Enter item ID to remove: ");
                if (remove_item(&inv, id)) {
                    printf(">> Item ID %d was removed from the inventory.\n", id);
                } else {
                    printf("Error: Item ID %d could not be found.\n", id);
                }
                break;
            }
            case 3: {
                int id = read_integer("Enter item ID to search: ");
                Item* item = find_item(&inv, id);
                if (item != NULL) {
                    printf(">> Item found in database:\n");
                    print_item(item);
                } else {
                    printf("Error: Item ID %d could not be found.\n", id);
                }
                break;
            }
            case 4:
                list_items(&inv);
                break;
            case 5:
                printf("Saving database to disk...\n");
                if (save_inventory_to_file(&inv, DATA_FILE)) {
                    printf(">> Data saved successfully to '%s'.\n", DATA_FILE);
                }
                printf("Powering down. Goodbye!\n");
                running = 0; // Breaks out of the loop
                break;
            default:
                printf("Invalid choice. Please select a valid menu option.\n");
        }
    }

    // Clean up allocated memory before exiting
    free_inventory(&inv);
    return EXIT_SUCCESS;
}