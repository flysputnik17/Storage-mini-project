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
char* print_item(Storage *storage,int item_id);
int main()
{
    Storage *new_storage = NULL;
    int num_of_shelvs = 0;
    int num_of_cells = 0;
    // int item_quantity = 0;
    // int shelf_index = 0;
    // int cell_index = 0;
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
        //printf("line is %s\n", line);
        while (line != NULL)
        {
           
            //printf("%s\n", line);
            // Extract the first and second words
            char *first_word = line;
            char *second_word = strtok(NULL, delimiter);
            //printf("the first word is %s\n", first_word);
            //printf("the second word is %s\n", second_word);
            char *item_quantity = strtok(NULL, delimiter);
            char *shelf_index = strtok(NULL, delimiter);
            char *cell_index = strtok(NULL, delimiter);
            //printf("quntity is %s, shelf-index is %s, cell-indes is %s\n", item_quantity, shelf_index, cell_index);

            if (second_word == NULL)
            {
                break;
            }
            

            if (strcmp(first_word, "_isc") == 0)
            {
                new_storage = create_storage(atoi(second_word));              // checking if the storage was created successfully if not exit progam
                num_of_shelvs = atoi(second_word);              //assigning the number of shelves acording to the second word that was provided from txt file
                
            }
            // printf("tessssst\n");
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
            // printf("test number 2\n");
            if(strcmp(first_word, "_add") == 0)
            {
                // printf("test add function\n");
                // printf("Adding item...\n");
                new_storage = add_item(new_storage, second_word, atoi(item_quantity), atoi(shelf_index), atoi(cell_index));
                if (add_item != NULL)
                {
                    printf("Item added:\n");
                    printf("Item name: %s\n",new_storage->shelf->cell->item->item_name);
                    printf("Item quantity: %d\n", new_storage->shelf->cell->item->quantity);
                    printf("Item on shelf num: %d\n",new_storage->shelf->current_shelf_index);
                    printf("Item on cell num: %d\n",new_storage->shelf->cell->current_cell_index);
                }
                else
                {
                    printf("Failed to add item\n");
                }
            }
            if (strcmp(first_word, "_pi") == 0)
            {
                
                int item_id = atoi(second_word);
                if(new_storage == NULL)
                {
                    print_item(new_storage, item_id);
                }
            }
            
            line = strtok(NULL, delimiter);
        }
    }

    fclose(file);
    return 0;
}

Storage *create_storage(int num_of_shelvs)
{
    if(num_of_shelvs <= 0)
    {
        //error_print(1);
        return NULL;
    }
    Storage *new_storage;
    new_storage = (Storage *)malloc(sizeof(Storage));                                   //creating the storage
    if(new_storage == NULL){
        printf("failed allocating memory for the Storage\n");
        return NULL;
    }
    new_storage->start_num_of_shelvs = num_of_shelvs;

    Shelf *current_shelf;
    for (int i = 0; i < num_of_shelvs; i++)                                                 //creating the linkedlist of shelves
    {
        current_shelf = (Shelf *)malloc(sizeof(Shelf));                                     // creating the shelf
        if (current_shelf == NULL)
        {
            printf("failed allocating memory for the shelf\n");
            free(new_storage);                                                          // freeing the memory of created storage in case of error allocating memory for shelf
            return NULL;
        }
        new_storage->shelf = current_shelf;                                              // assigning the created shelf to the storage
        current_shelf = current_shelf->next;                                                                // moving to the next shelf
    }
    new_storage->shelf->current_shelf_index = 0;

    return new_storage;
}

Storage *create_cells(Storage *storage, int num_of_cells, int shelf_index)
{
    
    if(shelf_index > storage->start_num_of_shelvs)                      //checking if the shelf index im resiving from the txt file is correct not equal or bigger thenthe start num of shelvs i was creating in the create_storage funciton
    {
        printf("Shelf index is out of bounds\n");
        return NULL;
    }

    Cell *current_cell;
    storage->shelf->num_of_cells = num_of_cells;                         //assigning the number of cells to the shelf
    for (int i = 0; i < num_of_cells; i++)                              //for loop to create the numebr of cells acording to data from txt file
    {
        current_cell = (Cell *)malloc(sizeof(Cell));
        if (current_cell == NULL)
        {
            printf("Failed allocating memory for the cells\n");
            while(current_cell != NULL)
            {
                free(current_cell->next);           //if failed free the memory of the cell that was created previously
            }
            return NULL;
        }
        storage->shelf->cell = current_cell;        //assigning the cell to the shelf
        current_cell = current_cell->next;
    }
    storage->shelf->cell->current_cell_index = 0;

    return storage;
}

Storage *add_item(Storage *storage, char *item_name, int quantity, int shelf_index, int cell_index)
{
    if (storage == NULL || shelf_index >= storage->start_num_of_shelvs || cell_index > storage->shelf->num_of_cells)
    {
        printf("Invalid shelf or cell index\n");
        return storage;
    }

    Item *new_item = (Item *)malloc(sizeof(Item));
    if (new_item == NULL)
    {
        printf("Failed allocating memory for the item\n");
        return storage;
    }

    // Initialize the item's fields
    new_item->item_id = cell_index;         //assigning the item id to the cell
    new_item->item_name = strdup(item_name);                                      
    new_item->quantity = quantity;
    if(new_item->quantity > (storage->shelf->num_of_cells - storage->shelf->cell->current_cell_index))
    {
        Shelf *new_shelf = (Shelf *)malloc(sizeof(Shelf));
        if(new_shelf == NULL)
        {
            printf("Failed allocating memory for the shelf\n");
            return storage;
        }
    
        storage->shelf->next = new_shelf;
        new_shelf->num_of_cells = quantity;
        new_shelf = new_shelf->next;
        storage->shelf->cell->item = new_item;
        new_item = new_item->next;
        return storage;
    }
    // Link the new item to the cell
    storage->shelf->cell->item = new_item;
    storage->shelf->cell->current_cell_index = new_item->item_id;   //giving the current cell index to be the item id
    new_item = new_item->next;
    return storage;
}


char *print_item(Storage *storage, int item_id)
{
    for (int i = 0; i < storage->start_num_of_shelvs; i++)
    {
        for (int j = 0; j < storage->shelf->num_of_cells; j++)
        {
            if (storage->shelf->cell->item->item_id == item_id)
            {
                printf("item name: %s\n", storage->shelf->cell->item->item_name);
                printf("item id: %d\n", storage->shelf->cell->item->item_id);
                printf("item position: [%d,%d]\n",storage->shelf->num_of_cells,storage->shelf->cell->item->item_id);
                return storage->shelf->cell->item->item_name;
            }
        }
    }
    return NULL;   
}
