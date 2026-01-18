#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PRODUCTS 100
#define MAX_STRING_LENGTH 50

typedef struct {
    int id;
    char name[MAX_STRING_LENGTH];
    char category[MAX_STRING_LENGTH];
    int quantity;
    float price;
} Product;

Product warehouse[MAX_PRODUCTS];
int productCount = 0;

void saveDatabase() {
    FILE *fp = fopen("database.csv", "w");
    if (fp == NULL) {
        perror("Error saving file");
        return;
    }

    for (int i = 0; i < productCount; i++) {
        fprintf(fp, "%d,%s,%s,%d,%.2f\n",
            warehouse[i].id,
            warehouse[i].name,
            warehouse[i].category,
            warehouse[i].quantity,
            warehouse[i].price);
    }
    fclose(fp);
    printf("Database saved successfully.\n");
}

int main() {
    bool isOn = true;
    FILE *fp;

    fp = fopen("database.csv", "r");
    if (fp != NULL) {
        while (fscanf(fp, "%d,%[^,],%[^,],%d,%f\n",
                &warehouse[productCount].id,
                warehouse[productCount].name,
                warehouse[productCount].category,
                &warehouse[productCount].quantity,
                &warehouse[productCount].price) == 5) {
            productCount++;
        }
        fclose(fp);
    }

    while (isOn) {
        int choice;

        printf("\n--- WAREHOUSE MANAGER ---\n");
        printf("[1] Print database\n");
        printf("[2] Add product\n");
        printf("[3] Modify product\n");
        printf("[4] Delete product\n");
        printf("[5] Search product\n");
        printf("[0] Save & Quit\n");
        printf("Select an option: ");
        scanf("%d", &choice);

        switch (choice) {
            case 0:
                saveDatabase();
                isOn = false;
                break;

            case 1:
                printf("\nID | Name | Category | Qty | Price\n");
                for (int i = 0; i < productCount; i++) {
                    printf("%d | %s | %s | %d | %.2f\n",
                        warehouse[i].id,
                        warehouse[i].name,
                        warehouse[i].category,
                        warehouse[i].quantity,
                        warehouse[i].price);
                }
                break;

            case 2:
                if (productCount >= MAX_PRODUCTS) {
                    printf("Error: Warehouse is full.\n");
                } else {
                    int newIndex = productCount;

                    if (newIndex > 0) {
                        warehouse[newIndex].id = warehouse[newIndex - 1].id + 1;
                    } else {
                        warehouse[newIndex].id = 1;
                    }

                    printf("Enter name: ");
                    scanf("%s", warehouse[newIndex].name);
                    printf("Enter category: ");
                    scanf("%s", warehouse[newIndex].category);
                    printf("Enter quantity: ");
                    scanf("%d", &warehouse[newIndex].quantity);
                    printf("Enter price: ");
                    scanf("%f", &warehouse[newIndex].price);

                    productCount++;
                    printf("Product added to memory (will be saved on exit).\n");
                }
                break;
            case 3:
                printf("Enter the id of the product you need to modify: \n");
                int idToModify;
                scanf("%d", &idToModify);
                for (int i = 0; i < productCount; i++) {
                    if (warehouse[i].id == idToModify) {
                        printf("Enter name: ");
                        scanf("%s", warehouse[i].name);
                        printf("Enter category: ");
                        scanf("%s", warehouse[i].category);
                        printf("Enter quantity: ");
                        scanf("%d", &warehouse[i].quantity);
                        printf("Enter price: ");
                        scanf("%f", &warehouse[i].price);
                    }
                }
                break;
            case 4:
                int idToDelete;
                printf("Enter the id of the product you need to delete: \n");
                scanf("%d", &idToDelete);

                int foundIndex = -1;
                for (int i = 0; i < productCount; i++) {
                    if (warehouse[i].id == idToDelete) {
                        foundIndex = i;
                    }
                }
                if (foundIndex != -1) {
                    warehouse[foundIndex] = warehouse[productCount-1];
                    productCount--;
                    printf("Product deleted successfully.\n");
                }
                else {
                    printf("Product does not exist.\n");
                }
                break;
            case 5:
                printf("Enter the id of the product you need to search: ");
                int idToSearch;
                scanf("%d", &idToSearch);
                for (int i = 0; i < productCount; i++) {
                    if (warehouse[i].id == idToSearch) {
                        printf("%d | %s | %s | %d | %.2f\n",
                        warehouse[i].id,
                        warehouse[i].name,
                        warehouse[i].category,
                        warehouse[i].quantity,
                        warehouse[i].price);
                    }
                }
                break;

            default:
                printf("Invalid selection.\n");
        }
    }

    return 0;
}