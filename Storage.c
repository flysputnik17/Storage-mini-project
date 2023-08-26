#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef enum {success,fail}Result;

typedef struct item{
    int item_id;
    char *item_name;
    int quantity;
    struct item *next;
} Item;

typedef struct cell{
    Item *item;
    struct cell *next;
} Cell;

typedef struct shelf{
    int *num_of_cells;
    struct shelf *next;
    Cell *cell;
} Shelf;

typedef struct storage{
    int start_num_of_shelvs;
    Shelf *shelf;
} Storage;

Result create_storage(int num_of_shelvs);
Result add_item(Storage *storage,char *item_name, int qunatity, int shelf_index, int cell_index);
Result create_cells(Storage *storage, int num_of_cells, int shelf_index);
char* print_item(Storage *storage,int item_id);
int main()
{
    Storage *new_storage = NULL;
    int current_shelf_index = 0;
    int error_print = 0;
    switch (error_print)
    {
        case 1:
            printf("Error- action does not exist!\n");
            break;
        case 2:
            printf("SWAP- one/two of the locations are not legal!\n");
            break;
        case 3:
            printf("MOVE- one/two of the locations are not legal!\n");
            break;
        case 4:
            printf("PRINT ITEM- the item number is not legal!\n");
            break;
        case 5:
            printf("REMOVE- the items location is not legal!\n");
            break;
    }

    //char *first_word, second_word;
    int num_of_shelvs = 0;
    int num_of_cells = 0;
    //int item_quantity = 0;
    //int shelf_index = 0;
    //int cell_index = 0;
    const char delimiter []= " ";


    FILE *file;
    file = fopen("Storage.txt","r");
    if(file == NULL)
    {
        printf("Error");
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
                if(create_storage(atoi(second_word)) == success)    //checking if the storage was created successfully if not exit progam
                {
                    num_of_shelvs = atoi(second_word);              //assigning the number of shelves acording to the second word that was provided from txt file
                }
                
            }
            printf("tessssst\n");
            if (strcmp(first_word, "_msps") == 0)
            {
                if(new_storage != NULL)
                {
                    num_of_cells = atoi(second_word);
                    for (int i = 0; i < num_of_cells; i++)              //a for loop for iterating through the number of shelfs to create the cells for each one of them
                    {
                        if(create_cells(new_storage, num_of_cells, current_shelf_index) == success)
                        {
                            current_shelf_index++;
                        }
                    }
                }
                
            }
            printf("test number 2\n");
            if(strcmp(first_word, "_add") == 0)
            {
                printf("test add function\n");
                if(add_item(new_storage, second_word, atoi(item_quantity), atoi(shelf_index), atoi(cell_index)) == success)
                {
                    printf("itme added : \nitem name is %s\nitem quantity is %s\nitem on shelf num : %d\n item on cell num : %d\n", new_storage->shelf->cell->item, new_storage->shelf->cell->item->item_name, current_shelf_index, new_storage->shelf->cell);
                    printf("test add function\n");
                }else{
                    printf("total fail\n");
                }
            }
            // if (strcmp(first_word, "pi") == 0)
            // {
            //    if(new_storage != NULL)
            //    {
            //        item_id = atoi(second_word);
            //    }
            // }
            
            line = strtok(NULL, delimiter);
        }
    }

    fclose(file);
    return 0;
}

Result create_storage(int num_of_shelvs)
{
    if(num_of_shelvs <= 0)
    {
        //error_print(1);
        return fail;
    }
    Storage *new_storage;
    new_storage = (Storage *)malloc(sizeof(Storage));      //creating the storage
    if(new_storage == NULL){
        printf("failed allocating memory for the Storage\n");
        return fail;
    }
    new_storage->start_num_of_shelvs = num_of_shelvs;

    Shelf *current_shelf;
    for (int i = 0; i < num_of_shelvs; i++)      //creating the linkedlist of shelves
    {
        current_shelf = (Shelf *)malloc(sizeof(Shelf)); // creating the shelf
        if (current_shelf == NULL)
        {
            printf("failed allocating memory for the shelf\n");
            free(new_storage); // freeing the memory of created storage in case of error allocating memory for shelf
            return fail;
        }
        new_storage->shelf = current_shelf; // assigning the created shelf to the storage
        current_shelf = current_shelf->next;    // moving to the next shelf
    }
    
    return success;
}

Result create_cells(Storage *storage, int num_of_cells, int shelf_index)
{
    
    if(shelf_index > storage->start_num_of_shelvs)      //checking if the shelf index im resiving from the txt file is correct not equal or bigger thenthe start num of shelvs i was creating in the create_storage funciton
    {
        printf("Shelf index is out of bounds\n");
        return fail;
    }

    Cell *current_cell;                         //pointing to the current cell
    for (int i = 0; i < num_of_cells; i++)      //for loop to create the numebr of cells acording to data from txt file
    {
        current_cell = (Cell *)malloc(sizeof(Cell) * num_of_cells);
        if (current_cell == NULL)
        {
            printf("Failed allocating memory for the cells\n");
            while(current_cell != NULL)
            {
                free(current_cell->next);           //if failed free the memory of the cell that was created previously
            }
            return fail;
        }
        storage->shelf->cell = current_cell;        //assigning the cell to the shelf
        current_cell = current_cell->next;         //moving to the next cell    
    }

    return success;
}

Result add_item(Storage *storage, char *item_name, int quantity, int shelf_index, int cell_index)
{
    // Check if the shelf index is valid
    if (shelf_index < 0 || shelf_index >= storage->start_num_of_shelvs)
    {
        printf("Shelf index is out of bounds\n");
        return fail;
    }

    // Check if the cell index is valid
    if (cell_index < 0 || cell_index >= sizeof(storage->shelf[shelf_index].num_of_cells))
    {
        printf("Cell index is out of bounds\n");
        return fail;
    }

    // Iterate through cells in the specified shelf
    int item_already_exists = 0; // Flag to track if an item with the same name already exists
    for (int i = 0; i < sizeof(storage->shelf[shelf_index].cell); i++)
    {
        // Check if the cell contains an item with the same name
        if (storage->shelf[shelf_index].cell[i].item != NULL &&
            strcmp(storage->shelf[shelf_index].cell[i].item->item_name, item_name) == 0)
        {
            item_already_exists = 1;
            break; // An item with the same name already exists, no need to continue searching
        }
    }

    // Allocate memory for the new item
    Item *new_item = (Item *)malloc(sizeof(Item));
    if (new_item == NULL)
    {
        printf("Failed allocating memory for the item\n");
        return fail;
    }

    // Initialize the item
    new_item->item_name = (char *)malloc(strlen(item_name) + 1);
    if (new_item->item_name == NULL)
    {
        printf("Failed allocating memory for the item name\n");
        free(new_item);
        return fail;
    }
    strcpy(new_item->item_name, item_name);

    // Set the item_id based on whether the item already exists or not
    if (item_already_exists)
    {
        // An item with the same name already exists in the shelf, so assign the same item_id
        new_item->item_id = storage->shelf[shelf_index].cell[cell_index].item->item_id;
    }
    else
    {
        // No item with the same name exists, increment the item_id
        new_item->item_id = storage->shelf[shelf_index].cell[cell_index].item->item_id + 1;
    }

    new_item->quantity = quantity;

    // Add the new item to the specified cell in the specified shelf
    storage->shelf[shelf_index].cell[cell_index].item = new_item;

    return success;
}

// Result add_item(Storage *storage,char *item_name,int qunatity,int shelf_index,int cell_index)
// {
    
//     if(cell_index > sizeof(storage->shelf->num_of_cells))       //checking if the cell index is out of bounds
//     {
//         printf("Cell index is out of bounds\n");
//         return fail;
//     }
//     if(shelf_index > storage->start_num_of_shelvs)  //cheking if the shelf index is out of bounds
//     {
//         printf("Shelf index is out of bounds\n");
//         return fail;
//     }

//     Item *new_item = (Item *)malloc(sizeof(Item));      //creating the item struct
//     if(new_item == NULL)
//     {
//         printf("Failed allocating memory for the item\n");
//         return fail;
//     }
//     Cell *find_item;
//     for(int i =0; i < sizeof(storage->shelf->num_of_cells); i++)
//     {
//         if(strcmp(storage->shelf->cell[i].item->item_name, item_name) != 0)     //iring the item name from the txt file to the item name in the cell
//         {
//             new_item->item_id++;                                    //if the item name is not found incrementing the item id to creating a unique item id
//         }
//     }
//     if(cell_index < sizeof(storage->shelf->num_of_cells))
//     {
//         cell_index++;                           //incrementing the cell index so the next item will be in the next cell
//     }
//     new_item->item_name = (char *)malloc(sizeof(strlen(item_name) + 1)); //allocating memmory for the item name
//     {
//         if(new_item->item_name == NULL)
//         {
//             printf("Failed allocating memory for the item name\n");
//             free(new_item);
//             return fail;
//         }
//         strcpy(new_item->item_name, item_name);     //copying the item name from the txt file to the newly created item
//         new_item->quantity = qunatity;              //copying the quantity from the txt file to the newly created item
//     }
//     return success;
// }

// char *print_item(Storage *storage, int item_id)
// {
//     Storage *head = storage->shelf->item.item_id;
//     while(head->shelf->item.next != NULL && head->shelf->item.item_id != item_id)
//     {
//         head = head->shelf->item.next;
//     }
    
// }
