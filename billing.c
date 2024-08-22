#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LIMIT 10 //We can only add these many items to the Stock

typedef struct
{
    int hsn;
    char itemName[30];
    float quantity;
    float price;
} Items;

int hash(char *key){
    int hash_value = 0;
    while(*key){
        hash_value += *key++;
    }
    return hash_value % MAX_LIMIT;
}

void initHashTable(Items *entries[]){
    for (int i = 0; i < MAX_LIMIT; i++)
    {
        entries[i] = NULL;
    }  
}

void addToStock(Items *entries[]){
    int hsn;
    char itemName[30], ch;
    float quantity;
    float price;
    printf("Please enter item details...\n");
    printf("Enter HSN-Code: ");
    scanf("%d", &hsn);
    for(int i=0; i < MAX_LIMIT; i++){
        if (entries[i] == NULL){
            continue;
        }
        if (entries[i]->hsn == hsn){
            printf("Item already exists with the same HSN-Code. Do you want to add quantity to this item?(press y/n) ");
            scanf(" %c", &ch);
            if (ch == 'y'){
                printf("Enter the Quantity you want to add: ");
                scanf("%f", &quantity);
                entries[i]->quantity += quantity;
            }
            return;
        }
    }
    printf("Enter Item-Name: ");
    scanf(" %[^\n]%*c", itemName);
    printf("Enter Quantity: ");
    scanf("%f", &quantity);
    printf("Enter Price: ");
    scanf("%f", &price);

    int index = hash(itemName);
    int i = index;

    while (entries[i] != NULL)
    {
        i = (i + 1) % MAX_LIMIT;
        if(i == index){
            printf("Stock Full! You cannot add items.\n");
            return;
        }
    }

    Items *new_item = (Items*)malloc(sizeof(Items));
    new_item->hsn = hsn;
    strcpy(new_item->itemName, itemName);
    new_item->price = price;
    new_item->quantity = quantity;

    entries[i] = new_item;
    printf("Item successfully added to the Stock!\n");
}

void deleteFromStock(Items *entries[]){
    int opt, hsn;
    float quantity;
    printf("Are you sure, you want to delete the item form Stock?(If Yes press 1, otherwise press 0) ");
    scanf("%d", &opt);
    if(opt == 0){
        return;
    }
    printf("Enter the HSN-Code: ");
    scanf("%d", &hsn);

    for(int i=0; i < MAX_LIMIT; i++){
        if (entries[i] == NULL){
            continue;
        }
        if (entries[i]->hsn == hsn){
            printf("1. If you want to delete the total stock present on this HSN-Code.(press 1)\n");
            printf("2. If you just want to adjust the quantity.(press 2)\n");
            scanf("%d", &opt);
            if (opt == 1){
                free(entries[i]);
                entries[i] = NULL;
                printf("Sucessfully deleted the item from the Stock!\n");
                return;
            }
            printf("Please enter the quantity you are returning present on this HSN-Code: ");
            scanf("%f", &quantity);
            entries[i]->quantity -= quantity;
            if (entries[i]->quantity < 0){
                printf("Quantity already present is lesser than you are requesting to be removed!\n");
                entries[i]->quantity += quantity;
                return;
            }
        }
    }

}

void viewStock(Items *entries[]){
    printf("----------------------------------------------------------------------\n");
    printf("%-15s %-25s %5s %15s\n", "HSN-Code", "Item-Name", "Quantity", "Price");
    printf("----------------------------------------------------------------------\n");
    for(int i=0; i < MAX_LIMIT; i++){
        if (entries[i] == NULL){
            continue;
        }
        printf("%-15d %-25s %5.1f %15.1f\n", entries[i]->hsn, entries[i]->itemName, entries[i]->quantity, entries[i]->price);
    }
    printf("---------------------------------------------------------------------\n\n");
}

void cart(Items *entries[]){
    int option;
    int i;
    int hsn;
    char itemName[30];
    float quantity, base_price;
    float total_qty = 0.0f, total_cost = 0.0f;
    int check = 0; 
    FILE *fptr;

    fptr = fopen("bill.txt", "w");
    fprintf(fptr, "                 EVERyTHING SHOPPING MALL\n");
    fprintf(fptr, "Opp. Sony digital, Nakkalgutta, Hanamkonda, Telangana-506345.\nPhone no.: +91 9218199111\nGSTIN: AB2929292Z833K\nPAN No.: A123456DK8K\n");
    fprintf(fptr, "----------------------------------------------------------------------------------\n");
    fprintf(fptr, "%-15s %-25s %-15s %-10s %15s\n", "HSN-Code", "item-name", "cost-per-item", "quantity", "cost");
    fprintf(fptr, "----------------------------------------------------------------------------------\n");
    fclose(fptr);
    fptr = fopen("bill.txt", "a");
    do
    {
        printf("choose an option:\n\t1)Add item\n\t2)Empty the Cart\n\t3)Generate bill\n\t4)Exit\n\t5)Print bill\n");
        scanf("%d", &option);
        switch (option){
            case 1:
                printf("Enter item details:\n");
                printf("Enter the HSN-Code: ");
                scanf("%d", &hsn);
                printf("Item Quantity: ");
                scanf("%f", &quantity);
                for(i=0; i < MAX_LIMIT; i++){
                    if (entries[i] == NULL){
                        continue;
                    }
                    if (entries[i]->hsn == hsn){
                        check = 1;
                        strcpy(itemName, entries[i]->itemName);
                        base_price = entries[i]->price;
                        entries[i]->quantity -= quantity;
                        break;
                    }
                }
                if (check == 0){
                    printf("Invalid HSN-Code!\n");
                    break;
                }
                if (entries[i]->quantity < 0){
                    printf("This item is out of stock!\n");
                    entries[i]->quantity += quantity;
                    break;
                }
                fprintf(fptr, "%-15d %-25s %15.1f %10.1f %15.1f\n", hsn, itemName, base_price, quantity, base_price*quantity);
                total_qty += quantity;
                total_cost += quantity*base_price;
                break;
            case 2:
                printf("Cart emptied!\n");
                break;
            case 3:
                fprintf(fptr, "----------------------------------------------------------------------------------\n");
                fprintf(fptr, "%-57s %10.1f %15.1f\n", "Taxable Amount", total_qty, total_cost);
                fprintf(fptr, "%-68s %15.1f\n", "CGST @2.5%", total_cost*2.5/100);
                fprintf(fptr, "%-68s %15.1f\n", "SGST @2.5%", total_cost*2.5/100);
                fprintf(fptr, "%-68s %15.1f\n", "Discount @5%", -total_cost*5/100);
                fprintf(fptr, "-----------------------------------------------------------------------------------\n");
                fprintf(fptr, "%-68s %15.1f\n", "Total Amount:", total_cost+total_cost*2.5/100+total_cost*2.5/100-total_cost*5/100);
                fprintf(fptr, "-----------------------------------------------------------------------------------\n");
                fprintf(fptr, "                                  Thank You! Visit Again.                          \n");
                fclose(fptr);
                printf("Bill generated\n");
                break;
            case 4:
                printf("Exited!\n");
                break;
            case 5:
                fptr = fopen("bill.txt", "r");
                int line;
                while ((line = fgetc(fptr)) != EOF) {
                    printf("%c", line);
                }

                fclose(fptr);
                break;
            default:
                printf("Enter valid option!\n");
            }
    } while (option != 4 && option != 5 && option != 2);
}

int main() {
    Items *entries[MAX_LIMIT];
    initHashTable(entries);
    int ch;
    do
    {
        printf("Main Menu\n\t1.Open Cart\n\t2.Add to Stock\n\t3.Delete from Stock\n\t4.View Current Stock\n\t5.Exit\n");
        printf("Choose an option: ");
        scanf("%d", &ch);
        switch (ch)
        {
        case 1:
            printf("=============================================CART OPENED=============================================\n");
            cart(entries);
            break;
        case 2:
            addToStock(entries);
            break;
        case 3:
            deleteFromStock(entries);
            break;
        case 4:
            viewStock(entries);
            break;
        case 5:
            printf("Exited!");
            break;
        default:
            printf("Choose a valid option!");
            break;
        }
    } while (ch != 5);
    

    return 0;
}