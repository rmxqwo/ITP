//e.bayazitov@innopolis.university
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// all item types available in the game
enum ItemType {
    WOOD,
    COBBLESTONE,
    COAL,
    STICK,
    WOODEN_PICKAXE,
    WOODEN_SWORD,
    TORCH,
    FURNACE
};

// structure for inventory items
struct InventoryItem {
    enum ItemType type;    // what kind of item
    int counter;           // how many we have
};

struct InventoryItem *inventory = NULL;    // dynamic array for inventory
int inventoryCapacity = 0;                 // max slots available
int usedSlots = 0;                         // currently used slots

// recipe structure for crafting
typedef struct {
    char name[50];                  // recipe name
    enum ItemType resultType;       // what we get
    int resultCount;                // how many we get
    enum ItemType ingredients[3];   // what we need
    int ingredientCounts[3];        // how many we need
    int numIngredients;             // how many ingredients total
} Recipe;

// convert enum to string for printing
const char* itemtype_to_string(enum ItemType type) {
    switch(type) {
        case WOOD: return "WOOD";
        case COBBLESTONE: return "COBBLESTONE";
        case COAL: return "COAL";
        case STICK: return "STICK";
        case WOODEN_PICKAXE: return "WOODEN_PICKAXE";
        case WOODEN_SWORD: return "WOODEN_SWORD";
        case TORCH: return "TORCH";
        case FURNACE: return "FURNACE";
        default: return "UNKNOWN";
    }
}

// find item index in inventory by type
int find_item_index(enum ItemType type){
    for(int i = 0; i < usedSlots; i++){
        if(inventory[i].type == type){
            return i;
        }
    }
    return -1;  // not found
}

// remove items from inventory
void remove_item(enum ItemType type, int count) {
    int index = find_item_index(type);
    if (index == -1) return;  // item not found
    
    inventory[index].counter -= count;
    
    // remove slot if counter goes to zero
    if (inventory[index].counter <= 0) {
        for (int i = index; i < usedSlots - 1; i++) {
            inventory[i] = inventory[i + 1];
        }
        usedSlots--;
    }
}

// check if we have enough resources to craft
int can_craft(Recipe recipe) {
    for (int i = 0; i < recipe.numIngredients; i++) {
        enum ItemType ingredient = recipe.ingredients[i];
        int required = recipe.ingredientCounts[i];
        int index = find_item_index(ingredient);
        
        if (index == -1 || inventory[index].counter < required) {
            return 0;  // not enough resources
        }
    }
    return 1;  // can craft
}

// double inventory capacity when full
void expand_inventory() {
    int newCapacity = inventoryCapacity * 2;
    struct InventoryItem *newInventory = realloc(inventory, newCapacity * sizeof(struct InventoryItem));
    inventory = newInventory;
    inventoryCapacity = newCapacity;
    printf("Inventory expanded, current capacity is %d\n", inventoryCapacity);
}

// add item to inventory
void add_item(enum ItemType type, int cnt) {
    int index = find_item_index(type);
    
    if (index != -1) {
        // item exists, just increase counter
        inventory[index].counter += cnt;
    } 
    else {
        // new item, need to add to inventory
        if (usedSlots >= inventoryCapacity) {
            expand_inventory();  // make space if needed
        }

        inventory[usedSlots].type = type;
        inventory[usedSlots].counter = cnt;
        usedSlots++;
    }
}

// main crafting function
void craft_item(Recipe recipe) {
    if (!can_craft(recipe)) {
        printf("Not enough resources for %s\n", recipe.name);
        return;
    }
    
    // remove ingredients from inventory
    for (int i = 0; i < recipe.numIngredients; i++) {
        enum ItemType ingredient = recipe.ingredients[i];
        int required = recipe.ingredientCounts[i];
        remove_item(ingredient, required);
    }
    
    // add result to inventory
    add_item(recipe.resultType, recipe.resultCount);
    printf("Successfully crafted %s\n", recipe.name);
}

// convert string to enum type
enum ItemType string_to_itemtype(const char *str) {
    if (strcmp(str, "WOOD") == 0) return WOOD;
    if (strcmp(str, "COBBLESTONE") == 0) return COBBLESTONE;
    if (strcmp(str, "COAL") == 0) return COAL;
    if (strcmp(str, "STICK") == 0) return STICK;
    if (strcmp(str, "WOODEN_PICKAXE") == 0) return WOODEN_PICKAXE;
    if (strcmp(str, "WOODEN_SWORD") == 0) return WOODEN_SWORD;
    if (strcmp(str, "TORCH") == 0) return TORCH;
    if (strcmp(str, "FURNACE") == 0) return FURNACE;
    return -1;  // unknown item
}

// initialize all crafting recipes
void initialize_recipes(Recipe *recipes) {
    // stick recipe - 2 wood = 4 sticks
    strcpy(recipes[0].name, "STICK");
    recipes[0].resultType = STICK;
    recipes[0].resultCount = 4;
    recipes[0].ingredients[0] = WOOD;
    recipes[0].ingredientCounts[0] = 2;
    recipes[0].numIngredients = 1;
    
    // wooden pickaxe recipe - 2 sticks + 3 wood
    strcpy(recipes[1].name, "WOODEN_PICKAXE");
    recipes[1].resultType = WOODEN_PICKAXE;
    recipes[1].resultCount = 1;
    recipes[1].ingredients[0] = STICK;
    recipes[1].ingredientCounts[0] = 2;
    recipes[1].ingredients[1] = WOOD;
    recipes[1].ingredientCounts[1] = 3;
    recipes[1].numIngredients = 2;
    
    // wooden sword recipe - 1 stick + 2 wood
    strcpy(recipes[2].name, "WOODEN_SWORD");
    recipes[2].resultType = WOODEN_SWORD;
    recipes[2].resultCount = 1;
    recipes[2].ingredients[0] = STICK;
    recipes[2].ingredientCounts[0] = 1;
    recipes[2].ingredients[1] = WOOD;
    recipes[2].ingredientCounts[1] = 2;
    recipes[2].numIngredients = 2;
    
    // torch recipe - 1 coal + 1 stick = 4 torches
    strcpy(recipes[3].name, "TORCH");
    recipes[3].resultType = TORCH;
    recipes[3].resultCount = 4;
    recipes[3].ingredients[0] = COAL;
    recipes[3].ingredientCounts[0] = 1;
    recipes[3].ingredients[1] = STICK;
    recipes[3].ingredientCounts[1] = 1;
    recipes[3].numIngredients = 2;
    
    // furnace recipe - 8 cobblestone
    strcpy(recipes[4].name, "FURNACE");
    recipes[4].resultType = FURNACE;
    recipes[4].resultCount = 1;
    recipes[4].ingredients[0] = COBBLESTONE;
    recipes[4].ingredientCounts[0] = 8;
    recipes[4].numIngredients = 1;
}

// comparison function for sorting inventory
int compare_items(const void *a, const void *b) {
    const struct InventoryItem *itemA = (const struct InventoryItem *)a;
    const struct InventoryItem *itemB = (const struct InventoryItem *)b;
    return strcmp(itemtype_to_string(itemA->type), 
                  itemtype_to_string(itemB->type));
}

int main(){
    FILE *file = fopen("input.txt", "r");
    if (file == NULL) {
        printf("Error opening file\n");
        return 1;
    }

    freopen("output.txt", "w", stdout);  // redirect output to file

    // initialize inventory and recipes
    inventoryCapacity = 3;
    inventory = malloc(inventoryCapacity * sizeof(struct InventoryItem));
    usedSlots = 0;
    Recipe recipes[5];
    initialize_recipes(recipes);

    // read initial 3 items from input file
    char type[1000];
    int count[3];
    for(int i = 0; i < 3; i++){
        fscanf(file, "%s", type);
        fscanf(file, "%d", &count[i]);
        enum ItemType itemType = string_to_itemtype(type);
        if(count[i] > 0){
            add_item(itemType, count[i]);  // add to inventory
        }
    }

    fgetc(file); // consume newline after initial items

    char s1[1000];  // command (CRAFT)
    char s2[1000];  // item to craft

    // process all craft commands from file
    while(fscanf(file, "%s %s", s1, s2) == 2){
        enum ItemType targetType = string_to_itemtype(s2);
        if (targetType == -1) {
            printf("Unknown recipe: %s\n", s2);
            continue;
        }
        
        // find recipe for this item
        int recipeIndex = -1;
        for (int i = 0; i < 5; i++) {
            if (recipes[i].resultType == targetType) {
                recipeIndex = i;
                break;
            }
        }
            
        if (recipeIndex != -1) {
            craft_item(recipes[recipeIndex]);  // try to craft
        } else {
            printf("Unknown recipe: %s\n", s2);
        }
    }

    // sort inventory alphabetically for nice output
    qsort(inventory, usedSlots, sizeof(struct InventoryItem), compare_items);
    
    // display final inventory state
    printf("Final inventory (%d/%d slots used):\n", usedSlots, inventoryCapacity);
    
    for (int i = 0; i < usedSlots; i++) {
        if (inventory[i].counter > 0) {
            printf("%s: %d\n", itemtype_to_string(inventory[i].type), inventory[i].counter);
        }
    }

    // cleanup
    free(inventory);
    fclose(file);

    return 0;
}