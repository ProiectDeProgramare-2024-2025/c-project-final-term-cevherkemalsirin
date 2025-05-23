#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <windows.h>

#define MAX_RECIPES 100
#define MAX_TITLE 50
#define MAX_DESC 200
#define MAX_INGREDIENTS 300

struct Recipe {
    char title[MAX_TITLE];
    char description[MAX_DESC];
    char ingredients[MAX_INGREDIENTS];
};

typedef struct Recipe Recipe;

Recipe recipes[MAX_RECIPES];
int recipe_count = 0;

void clearScreen() { system("cls"); }

void pauseScreen() {
    printf("\nPress Enter to return...");
    getchar(); getchar();
    clearScreen();
}

void displayMenu() {
    clearScreen();
    printf("\n \x1b[3m--- Recipe Application ---\x1b[0m\n");
    printf("\n 1. View the Recipes");
    printf("\n 2. Add a Recipe");
    printf("\n 3. Edit a Recipe");
    printf("\n 4. Delete a Recipe");
    printf("\n 5. Search a Recipe");
    printf("\n 6. Exit");
    printf("\n Choose from menu : ");
}

void SaveRecipesToFile() {
    FILE* fp = fopen("recipes.txt", "w");
    if (!fp) {
        printf("Error opening file for writing!\n");
        return;
    }
    for (int i = 0; i < recipe_count; i++) {
        fprintf(fp, "%s\n%s\n%s\n\n", 
            recipes[i].title,
            recipes[i].description,
            recipes[i].ingredients);
    }
    fclose(fp);
}

void LoadRecipesFromFile() {
    FILE* fp = NULL;
    errno_t err = fopen_s(&fp, "recipes.txt", "r");
    if (err != 0 || fp == NULL) {
        err = fopen_s(&fp, "recipes.txt", "w");
        fclose(fp);
        return;
    }

    recipe_count = 0;
    while (recipe_count < MAX_RECIPES) {
        if (fgets(recipes[recipe_count].title, MAX_TITLE, fp) == NULL)
            break;
        char* pos = strchr(recipes[recipe_count].title, '\n');
        if (pos) *pos = '\0';

        if (fgets(recipes[recipe_count].description, MAX_DESC, fp) == NULL)
            break;
        pos = strchr(recipes[recipe_count].description, '\n');
        if (pos) *pos = '\0';

        if (fgets(recipes[recipe_count].ingredients, MAX_INGREDIENTS, fp) == NULL)
            break;
        pos = strchr(recipes[recipe_count].ingredients, '\n');
        if (pos) *pos = '\0';

        char buffer[10];
        fgets(buffer, sizeof(buffer), fp);

        recipe_count++;
    }
    fclose(fp);
}


void viewRecipes(bool shouldPause, bool canSeeDetails) {
    clearScreen();

    printf("\n \x1b[3m--- Recipe List ---\x1b[0m\n");
    for (int i = 0; i < recipe_count; i++)
    {
        printf("%d. \x1b[33m%s\x1b[0m\n", i + 1, recipes[i].title);
    }
    if (recipe_count == 0)
    {
        printf("\x1b[31mNo recipes available.\x1b[0m\n");
    }
    else if (canSeeDetails)
    {
        int choice;
        printf("Enter recipe number to see details (or enter 0 to go back): ");
        scanf_s("%d", &choice);
        getchar();
        if (choice == 0) {
            if (shouldPause) pauseScreen();
            return;
        }
        if (choice < 1 || choice > recipe_count)
        {
            printf("\x1b[31mPlease enter a valid recipe number!\x1b[0m\n");
        }
        else
        {
            printf("\n\x1b[1mViewing Recipe:\x1b[0m\n \n");
            printf("\x1b[4mTitle\x1b[0m: \x1b[32m%s\x1b[0m\n", recipes[choice - 1].title);
            printf("\x1b[4mDescription\x1b[0m: \x1b[36m%s\x1b[0m\n", recipes[choice - 1].description);
            printf("\x1b[4mIngredients\x1b[0m: \x1b[35m%s\x1b[0m\n", recipes[choice - 1].ingredients);
        }
    }
    if (shouldPause)
        pauseScreen();
}

void addRecipe() {
    clearScreen();
    if (recipe_count >= MAX_RECIPES) {
        printf("\x1b[31mRecipe list is full!\x1b[0m\n");
        pauseScreen();
        return;
    }
    printf("\n \x1b[3m--- Add a Recipe ---\x1b[0m\n");
    printf("\n\nEnter recipe \x1b[33mtitle\x1b[0m: ");
    getchar();
    fgets(recipes[recipe_count].title, MAX_TITLE, stdin);
    char* context;
    strtok_s(recipes[recipe_count].title, "\n", &context);

    printf("Enter \x1b[33mdescription\x1b[0m for the recipe: ");
    fgets(recipes[recipe_count].description, MAX_DESC, stdin);
    strtok_s(recipes[recipe_count].description, "\n", &context);

    printf("Enter \x1b[33mingredients\x1b[0m for the recipe: ");
    fgets(recipes[recipe_count].ingredients, MAX_INGREDIENTS, stdin);
    strtok_s(recipes[recipe_count].ingredients, "\n", &context);

    recipe_count++;
    printf("\x1b[32mRecipe added successfully!\x1b[0m\n");
    SaveRecipesToFile();
    pauseScreen();
}

void editRecipe() {
    clearScreen();
    if (recipe_count == 0) {
        printf("\x1b[31mNo recipes to edit.\x1b[0m\n");
        pauseScreen();
        return;
    }
    viewRecipes(false, false);
    int choice;
    printf("Enter recipe number to edit: ");
    scanf_s("%d", &choice);
    getchar();
    if (choice < 1 || choice > recipe_count) {
        printf("\x1b[31mPlease enter a valid recipe number!\x1b[0m\n");
        pauseScreen();
        return;
    }
    printf("\n \x1b[3m--- Editing the Recipe ---\x1b[0m\n");
    printf("\x1b[33m%s\x1b[0m\nNew \x1b[35mtitle\x1b[0m: ", recipes[choice - 1].title);
    fgets(recipes[choice - 1].title, MAX_TITLE, stdin);
    char* context;
    strtok_s(recipes[choice - 1].title, "\n", &context);

    printf("Enter new \x1b[35mdescription\x1b[0m: ");
    fgets(recipes[choice - 1].description, MAX_DESC, stdin);
    strtok_s(recipes[choice - 1].description, "\n", &context);

    printf("Enter new \x1b[35mingredients\x1b[0m: ");
    fgets(recipes[choice - 1].ingredients, MAX_INGREDIENTS, stdin);
    strtok_s(recipes[choice - 1].ingredients, "\n", &context);

    printf("\x1b[32mRecipe updated!\x1b[0m\n");
    SaveRecipesToFile();
    pauseScreen();
}

void deleteRecipe() {
    clearScreen();
    if (recipe_count == 0) {
        printf("\x1b[31mNo recipes to delete.\x1b[0m\n");
        pauseScreen();
        return;
    }
    viewRecipes(false, false);
    int choice;
    printf("Enter recipe number to delete: ");
    scanf_s("%d", &choice);
    getchar();
    if (choice < 1 || choice > recipe_count) {
        printf("\x1b[31mInvalid choice!\x1b[0m\n");
        pauseScreen();
        return;
    }
    for (int i = choice - 1; i < recipe_count - 1; i++) {
        recipes[i] = recipes[i + 1];
    }
    recipe_count--;
    printf("\x1b[32mRecipe deleted!\x1b[0m\n");
    SaveRecipesToFile();
    pauseScreen();
}

void searchRecipe() {
    clearScreen();
    if (recipe_count == 0) {
        printf("\x1b[31mNo recipes available.\x1b[0m\n");
        pauseScreen();
        return;
    }
    printf("\n \x1b[3m--- Search a Recipe ---\x1b[0m\n");
    printf("\x1b[0m\nEnter keyword: ");
    getchar();
    char keyword[MAX_TITLE];
    fgets(keyword, MAX_TITLE, stdin);
    char* context;
    strtok_s(keyword, "\n", &context);
    bool found = false;
    for (int i = 0; i < recipe_count; i++) {
        if (strstr(recipes[i].title, keyword)) {
            printf("%d. \x1b[33m%s\x1b[0m\n", i + 1, recipes[i].title);
            found = true;
        }
    }
    if (!found) {
        printf("\x1b[31mNo matching recipes found.\x1b[0m\n");
    }
    pauseScreen();
}

void TakeInput(int* choice) {
    displayMenu();
    scanf_s("%d", choice);
    switch (*choice) {
    case 1: 
    {
        viewRecipes(true, true);
    }
    break;
    case 2:
    {
        addRecipe();
    }break;
    case 3:
    {
        editRecipe();
    }
        break;
    case 4:
    {
        deleteRecipe();
    }
    break;
    case 5:
    {
        searchRecipe(); 
    }
    break;
    case 6: break;
    default:
    {
        printf("Bad Input, try again!\n");
    }
    }
}



int main() {
    
    int choice;
    LoadRecipesFromFile(); 
    do {
        TakeInput(&choice);
    } while (choice != 6);
    return 0;
}
