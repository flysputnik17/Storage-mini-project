#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct item{
    int item_id;
    char *item_name;
    int quantity;
    struct item *next;
} Item;

typedef struct cell{
    int current_cell_index;
    Item *item;
    struct cell *next;
} Cell;

typedef struct shelf{
    int num_of_cells;
    int current_shelf_index;
    struct shelf *next;
    Cell *cell;
} Shelf;

typedef struct storage{
    int start_num_of_shelvs;
    Shelf *shelf;
} Storage;

Storage *create_storage(int num_of_shelvs);
Storage *add_item(Storage *storage,char *item_name, int qunatity, int shelf_index, int cell_index);
Storage *create_cells(Storage *storage, int num_of_cells, int shelf_index);
Storage *remove_item(Storage *storage, int shelf_index, int start_cell_index, int end_cell_index);
Storage *move_items(Storage *storage, int shelf_index, int start_cell_index, int end_cell_index,int new_shelf_index,int new_cell_index);
Storage *swap_items(Storage *storage, int shelf_index_a, int cell_index_a, int shelf_index_b, int cell_index_b);
char *item_print(Storage *storage,int item_id);

int main()
{
    Storage *new_storage = NULL;
    int num_of_shelvs = 0;
    int num_of_cells = 0;

    const char delimiter []= " ";


    FILE *file;
    file = fopen("Storage.txt","r");
    if(file == NULL)
    {
        return 1;
    }
    printf("file is open\n");
    


    while (!feof(file))
    {
        char character[100];
        fgets(character, sizeof(character), file);
        if (character[0] == '#' && character[0] != '\n') // here i make sure to ignore the line starting with #
        {
            continue;
        }

        char *line = strtok(character, delimiter);
        
        while (line != NULL)
        {
            char *first_word = line;
            char *second_word = strtok(NULL, delimiter);
            char *third_word = strtok(NULL, delimiter);
            char *fourth_word = strtok(NULL, delimiter);
            char *fifth_word = strtok(NULL, delimiter);
            char *sixth_word = strtok(NULL, delimiter);

            if (second_word == NULL)
            {
                break;
            }
            

            if (strcmp(first_word, "_isc") == 0)
            {
                num_of_shelvs = atoi(second_word);                            // assigning the number of shelves acording to the second word that was provided from txt file
                new_storage = create_storage(num_of_shelvs);                  // checking if the storage was created successfully if not exit progam
            }
            
            if (strcmp(first_word, "_msps") == 0)
            {
                if(new_storage != NULL)
                {
                    num_of_cells = atoi(second_word);
                    for (int i = 0; i < num_of_cells; i++)              //a for loop for iterating through the number of shelfs to create the cells for each one of them
                    {
                        new_storage = create_cells(new_storage, num_of_cells, num_of_shelvs);
                    }
                }
                
            }
            
            if(strcmp(first_word, "_add") == 0)
            {
                int item_quantity = atoi(third_word);
                int shelf_index = atoi(fourth_word);
                int cell_index = atoi(fifth_word);
                new_storage = add_item(new_storage, second_word, item_quantity, shelf_index,cell_index);
                if (add_item != NULL)
                {
                    printf("Item added:\n");
                    // printf("Item name: %s\n",new_storage->shelf->cell->item->item_name);
                    // printf("Item quantity: %d\n", new_storage->shelf->cell->item->quantity);
                    // printf("Item on shelf num: %d\n",new_storage->shelf->current_shelf_index);
                    // printf("Item on cell num: %d\n",new_storage->shelf->cell->current_cell_index);
                }
                else
                {
                    printf("Failed to add item\n");
                }
            }

            if(strcmp(first_word,"_rm") == 0)
            {
                int shelf_index = atoi(second_word);
                int start_cell_index = atoi(third_word);
                int end_cell = atoi(fourth_word);
                new_storage = remove_item(new_storage,shelf_index,start_cell_index,end_cell);
                printf("Item removed\n");
            }

            if(strcmp(first_word,"_mv") == 0)
            {
                int shelf_index = atoi(second_word);
                int cell_a_start = atoi(third_word);
                int cell_a_end = atoi(fourth_word);
                int new_shelf = atoi(fifth_word);
                int new_cell = atoi(sixth_word);
                new_storage = move_items(new_storage, shelf_index, cell_a_start, cell_a_end, new_shelf,new_cell);
                printf("Item moved\n");
            }
            if(strcmp(first_word,"_sw") == 0)
            {
                int shelf_a = atoi(second_word);
                int cell_a = atoi(third_word);
                int shelf_b = atoi(fourth_word);
                int cell_b = atoi(fifth_word);
                new_storage = swap_items(new_storage, shelf_a, cell_a, shelf_b,cell_b);
                printf("Item swapped\n");
            }

            // if (strcmp(first_word,"_pi") == 0)
            // {
            //     continue;
            //     if (new_storage != NULL)
            //     {
            //         item_print(new_storage, atoi(second_word));
            //     }
            // }
            // printf("number of items :%d\n", new_storage->shelf->num_of_cells);
            // printf("number of shelves :%d\n", new_storage->start_num_of_shelvs);
            line = strtok(NULL, "\n");
        }
    }

    fclose(file);

    return 0;
}

Storage *create_storage(int num_of_shelves)
{
    Storage *new_storage = (Storage *)malloc(sizeof(Storage)); // Allocate memory for Storage
    if (new_storage == NULL)
    {
        printf("Failed to allocate memory for Storage\n");
        return NULL;
    }

    new_storage->start_num_of_shelvs = num_of_shelves;
    new_storage->shelf = NULL; // Initialize the shelf linked list to be empty

    // Create the linked list of shelves
    Shelf *current_shelf = NULL;
    Shelf *prev_shelf = NULL;
    for (int i = 0; i < num_of_shelves; i++)
    {
        current_shelf = (Shelf *)malloc(sizeof(Shelf)); // Allocate memory for the current shelf
        if (current_shelf == NULL)
        {
            printf("Failed to allocate memory for Shelf\n");

            // Clean up previously allocated shelves
            while (new_storage->shelf != NULL)
            {
                current_shelf = new_storage->shelf->next;
                free(new_storage->shelf);
                new_storage->shelf = current_shelf;
            }

            free(new_storage); // Free the Storage structure
            return NULL;
        }

        // Initialize the shelf fields
        current_shelf->num_of_cells = 0; // Initialize the number of cells to 0
        current_shelf->current_shelf_index = i;
        current_shelf->next = NULL;
        current_shelf->cell = NULL; // Initialize the cell linked list to be empty

        if (prev_shelf == NULL)
        {
            new_storage->shelf = current_shelf; // If it's the first shelf, set it as the head of the linked list
        }
        else
        {
            prev_shelf->next = current_shelf; // Link the previous shelf to the current shelf
        }

        prev_shelf = current_shelf;
    }

    return new_storage;
}

Storage *create_cells(Storage *storage, int num_of_cells, int shelf_index)
{

    Shelf *target_shelf = &(storage->shelf[shelf_index]); // Get the target shelf

    Cell *current_cell = NULL;
    Cell *prev_cell = NULL;

    for (int i = 0; i < num_of_cells; i++)
    {
        current_cell = (Cell *)malloc(sizeof(Cell)); // Allocate memory for the current cell
        if (current_cell == NULL)
        {
            printf("Failed to allocate memory for Cell\n");

            // Clean up previously allocated cells
            while (target_shelf->cell != NULL)
            {
                prev_cell = target_shelf->cell;
                target_shelf->cell = target_shelf->cell->next;
                free(prev_cell);
            }

            return storage;
        }

        // Initialize the cell fields
        current_cell->current_cell_index = i;
        current_cell->item = NULL; // Initialize the item pointer to NULL
        current_cell->next = NULL;

        if (prev_cell == NULL)
        {
            target_shelf->cell = current_cell; // If it's the first cell, set it as the head of the linked list
        }
        else
        {
            prev_cell->next = current_cell; // Link the previous cell to the current cell
        }

        prev_cell = current_cell;
    }

    target_shelf->num_of_cells = num_of_cells; // Update the number of cells in the shelf

    return storage;
}

Storage *add_item(Storage *storage, char *item_name, int quantity, int shelf_index, int cell_index)
{

    // Handle the case when shelf_index is -1
    if (shelf_index == -1)
    {
        Shelf *current_shelf = storage->shelf;
        while (current_shelf != NULL)
        {
            current_shelf->current_shelf_index = 0;
            int space_available = current_shelf->num_of_cells - current_shelf->cell->current_cell_index;
            if (space_available >= quantity)
            {
                // Found a shelf with enough space, break the loop
                break;
            }
            current_shelf = current_shelf->next;
        }

        // If no suitable shelf is found, create a new one
        if (current_shelf == NULL)
        {
            Shelf *new_shelf = (Shelf *)malloc(sizeof(Shelf));
            if (new_shelf == NULL)
            {
                printf("Failed to allocate memory for a new shelf\n");
                return storage;
            }
            new_shelf->num_of_cells = quantity;
            new_shelf->next = NULL;
            new_shelf->cell = (Cell *)malloc(sizeof(Cell));

            // Link the new shelf to the end of the existing shelves
            Shelf *last_shelf = storage->shelf;
            while (last_shelf->next != NULL)
            {
                last_shelf = last_shelf->next;
            }
            last_shelf->next = new_shelf;
            current_shelf = new_shelf;
        }

        shelf_index = current_shelf->current_shelf_index;
        storage->start_num_of_shelvs++;
    }

    // Handle the case when cell_index is -1
    if (cell_index == -1)
    {
        // Find the first cell with enough space on the target shelf
        Shelf *target_shelf = &(storage->shelf[shelf_index]);
        int i = 0;

        // You should loop through the cells in the shelf
        Cell *target_cell = &(target_shelf->cell[i]);
        while (i <= target_shelf->num_of_cells - quantity)
        {
            if (target_cell->item == NULL)
            {
                int space_available = target_shelf->num_of_cells - i;
                if (space_available >= quantity)
                {
                    // Found a cell with enough space, break the loop
                    break;
                }
            }
            i++;
            target_cell = &(target_shelf->cell[i]);
        }

        // If no suitable cell is found, return an error
        if (i > target_shelf->num_of_cells - quantity)
        {
            printf("No suitable cell found on the shelf for the item\n");
            return storage;
        }

        cell_index = i;

        cell_index = i;
    }

    // Check if there is enough space in the target cell to store the item
    Cell *target_cell = &(storage->shelf[shelf_index].cell[cell_index]);
    int space_available = storage->shelf[shelf_index].num_of_cells - cell_index;

    if (space_available < quantity)
    {
        // Not enough space in the target cell, so look for another shelf
        Shelf *current_shelf = storage->shelf;
        while (current_shelf->next != NULL)
        {
            current_shelf = current_shelf->next;
            space_available = current_shelf->num_of_cells;
            if (space_available >= quantity)
            {
                // Found a shelf with enough space, break the loop
                break;
            }
        }

        // If no suitable shelf is found, create a new one
        if (space_available < quantity)
        {
            Shelf *new_shelf = (Shelf *)malloc(sizeof(Shelf));
            if (new_shelf == NULL)
            {
                printf("Failed to allocate memory for a new shelf\n");
                return storage;
            }
            new_shelf->num_of_cells = quantity;
            new_shelf->next = NULL;
            new_shelf->cell = NULL;
            current_shelf->next = new_shelf;
            current_shelf = new_shelf;
        }
    }

    // Create and initialize the new item
    Item *new_item = (Item *)malloc(sizeof(Item));
    if (new_item == NULL)
    {
        printf("Failed to allocate memory for the item\n");
        return storage;
    }

    new_item->item_id = cell_index;
    new_item->item_name = strdup(item_name);
    new_item->quantity = quantity;
    new_item->next = NULL;

    // Set the item field of the target cell to the new_item
    storage->shelf[shelf_index].cell[cell_index].item = new_item;

    return storage;
}

char *item_print(Storage *storage, int item_id)
{
    for (int i = 0; i < storage->start_num_of_shelvs; i++)
    {
        Shelf *current_shelf = &(storage->shelf[i]); // Get the current shelf

        for (int j = 0; j < current_shelf->num_of_cells; j++)
        {
            Cell *current_cell = &(current_shelf->cell[j]); // Get the current cell

            Item *item = current_cell->item;

            while (item != NULL)
            {
                if (item->item_id == item_id)
                {
                    printf("item: %s, Number: %d, Position [%d,%d]\n", item->item_name, item->quantity, i, j);
                    return item->item_name;
                }
                item = item->next; // Move to the next item in the cell
            }
        }
    }

    printf("Item with id %d not found\n", item_id);
    return NULL;
}

Storage *remove_item(Storage *storage, int shelf_index, int start_cell_index, int end_cell_index)
{
    Shelf *target_shelf = &(storage->shelf[shelf_index]); // Get the target shelf

    for (int i = start_cell_index; i <= end_cell_index; i++)
    {
        Cell *target_cell = &(target_shelf->cell[i]); // Get the target cell

        Item *current_item = target_cell->item;
        while (current_item != NULL)
        {
            Item *next_item = current_item->next; // Store the next item temporarily
            free(current_item->item_name);        // Free the item name memory
            free(current_item);                   // Free the item structure itself
            target_cell->item = next_item;        // Update the cell to the next item
            current_item = next_item;             // Move to the next item in the cell
        }
    }

    return storage;
}

Storage *move_items(Storage *storage, int shelf_index, int start_cell_index, int end_cell_index, int new_shelf_index, int new_cell_index)
{
    Shelf *source_shelf = &(storage->shelf[shelf_index]);          // Get the source shelf
    Shelf *destination_shelf = &(storage->shelf[new_shelf_index]); // Get the destination shelf

    if (source_shelf == destination_shelf && shelf_index == new_shelf_index)
    {
        printf("Source and destination shelves are the same.\n");
        return storage;
    }

    for (int i = start_cell_index; i <= end_cell_index; i++)
    {
        Cell *source_cell = &(source_shelf->cell[i]);                        // Get the source cell
        Cell *destination_cell = &(destination_shelf->cell[new_cell_index]); // Get the destination cell

        Item *current_item = source_cell->item;
        while (current_item != NULL)
        {
            Item *next_item = current_item->next; // Store the next item temporarily

            // Move the item to the destination cell
            current_item->item_id = new_cell_index;
            current_item->next = destination_cell->item;
            destination_cell->item = current_item;

            source_cell->item = next_item; // Update the source cell to the next item
            current_item = next_item;      // Move to the next item in the source cell
        }
    }

    return storage;
}

Storage *swap_items(Storage *storage, int shelf_index_a, int cell_index_a, int shelf_index_b, int cell_index_b)
{
    Shelf *shelf_a = &(storage->shelf[shelf_index_a]); // Get shelf A
    Shelf *shelf_b = &(storage->shelf[shelf_index_b]); // Get shelf B

    Cell *cell_a = &(shelf_a->cell[cell_index_a]); // Get cell A
    Cell *cell_b = &(shelf_b->cell[cell_index_b]); // Get cell B

    // Swap items between cell A and cell B
    Item *temp = cell_a->item;
    cell_a->item = cell_b->item;
    cell_b->item = temp;

    return storage;
}

