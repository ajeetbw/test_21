/**
 * @file file_io.c
 * @brief Implementation of file input/output functions.
 */
#include <stdio.h>
#include "file_io.h"

int save_inventory_to_file(const Inventory* inv, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        fprintf(stderr, "Error: Could not open file %s for writing.\n", filename);
        return 0;
    }

    // Write the total count first to easily pre-allocate on load
    fprintf(file, "%d\n", inv->count);
    
    // Write out each item in a comma-separated format
    for (int i = 0; i < inv->count; i++) {
        fprintf(file, "%d,%s,%d,%.2f\n",
                inv->items[i].id,
                inv->items[i].name,
                inv->items[i].quantity,
                inv->items[i].price);
    }

    fclose(file);
    return 1;
}

int load_inventory_from_file(Inventory* inv, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        // Normal behavior on first run
        return 0; 
    }

    int count;
    if (fscanf(file, "%d\n", &count) != 1) {
        fclose(file);
        return 0; // File is poorly formatted or empty
    }

    // Read lines until EOF or expected count is reached
    for (int i = 0; i < count; i++) {
        Item item;
        // Read up to 99 characters until a comma is found to prevent buffer overflow
        if (fscanf(file, "%d,%99[^,],%d,%lf\n",
                   &item.id, item.name, &item.quantity, &item.price) == 4) {
            add_item(inv, item);
        }
    }

    fclose(file);
    return 1;
}