#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct items {
    char item[20];
    float price;
    int qty;
};

struct orders {
    char customer[50];
    char date[50];
    int numofItems;
    struct items itm[50];
};

void BillBana(char name[50], char date[30]) {
    printf("\n\n");
    printf("\t\t\t\t    RESTAURANT");
    printf("\n----------------------------------------------------------------------------");
    printf("\nDate: %s", date);
    printf("\nInvoice to: %s", name);
    printf("\n----------------------------------------------------------------------------");
    printf("\nItem\t\tQuantity\tPrice");
    printf("\n----------------------------------------------------------------------------\n");
}

void Kaisa_Banau_Bill(char item[20], int qty, float price) {
    printf("%s\t\t%d\t\t%.2f\n", item, qty, qty * price);
}

void Dekh_Bill(float total) {
    float dis = 0.1f * total;
    float netTotal = total - dis;
    float gst = 0.18f * netTotal;
    float grandTotal = netTotal + gst;

    printf("----------------------------------------------------------------------------\n");
    printf("Sub Total:\t\t\t%.2f\n", total);
    printf("Discount @10%%:\t\t\t%.2f\n", dis);
    printf("Net Total:\t\t\t%.2f\n", netTotal);
    printf("GST @18%%:\t\t\t%.2f\n", gst);
    printf("----------------------------------------------------------------------------\n");
    printf("Grand Total:\t\t\t%.2f\n", grandTotal);
    printf("----------------------------------------------------------------------------\n");
}

int main() {
    int opt, n, i;
    char saveBill;
    char name[50];
    float total;
    struct orders ord;
    FILE *fp;

    while (1) {
        printf("\n============================== RESTAURANT ==============================");
        printf("\n1. Generate Invoice");
        printf("\n2. Show all Invoices");
        printf("\n3. Search Invoice");
        printf("\n4. Exit");

        printf("\n\nYour Choice: ");
        scanf("%d", &opt);
        getchar();  // Clear input buffer

        switch (opt) {
            case 1:
                printf("\nEnter Customer Name: ");
                fgets(ord.customer, 50, stdin);
                ord.customer[strcspn(ord.customer, "\n")] = 0;  // Remove newline
                strcpy(ord.date, __DATE__);

                printf("Enter number of items: ");
                scanf("%d", &ord.numofItems);
                getchar();  // Clear input buffer

                total = 0;
                for (i = 0; i < ord.numofItems; i++) {
                    printf("\nItem %d Name: ", i + 1);
                    fgets(ord.itm[i].item, 20, stdin);
                    ord.itm[i].item[strcspn(ord.itm[i].item, "\n")] = 0;

                    printf("Quantity: ");
                    scanf("%d", &ord.itm[i].qty);
                    printf("Price: ");
                    scanf("%f", &ord.itm[i].price);
                    getchar();  // Clear newline

                    total += ord.itm[i].qty * ord.itm[i].price;
                }

                BillBana(ord.customer, ord.date);
                for (i = 0; i < ord.numofItems; i++) {
                    Kaisa_Banau_Bill(ord.itm[i].item, ord.itm[i].qty, ord.itm[i].price);
                }
                Dekh_Bill(total);

                printf("\nDo you want to save the invoice? [y/n]: ");
                scanf(" %c", &saveBill);
                getchar();

                if (saveBill == 'y' || saveBill == 'Y') {
                    fp = fopen(" RESTAURANT.txt", "a");
                    if (fp == NULL) {
                        printf("Error opening file.\n");
                        break;
                    }
                    if (fwrite(&ord, sizeof(struct orders), 1, fp)) {
                        printf("Invoice saved successfully.\n");
                    } else {
                        printf("Error saving invoice.\n");
                    }
                    fclose(fp);
                }
                break;

            case 2:
                fp = fopen(" RESTAURANT.txt", "r");
                if (fp == NULL) {
                    printf("Error opening file.\n");
                    break;
                }

                printf("\n*** Previous Invoices ***\n");
                while (fread(&ord, sizeof(struct orders), 1, fp)) {
                    float tot = 0;
                    BillBana(ord.customer, ord.date);
                    for (i = 0; i < ord.numofItems; i++) {
                        Kaisa_Banau_Bill(ord.itm[i].item, ord.itm[i].qty, ord.itm[i].price);
                        tot += ord.itm[i].qty * ord.itm[i].price;
                    }
                    Dekh_Bill(tot);
                }
                fclose(fp);
                break;

            case 3:
                printf("\nEnter Customer Name to search: ");
                fgets(name, 50, stdin);
                name[strcspn(name, "\n")] = 0;

                fp = fopen(" RESTAURANT.txt", "r");
                if (fp == NULL) {
                    printf("Error opening file.\n");
                    break;
                }

                int found = 0;
                while (fread(&ord, sizeof(struct orders), 1, fp)) {
                    if (strcmp(ord.customer, name) == 0) {
                        float tot = 0;
                        BillBana(ord.customer, ord.date);
                        for (i = 0; i < ord.numofItems; i++) {
                            Kaisa_Banau_Bill(ord.itm[i].item, ord.itm[i].qty, ord.itm[i].price);
                            tot += ord.itm[i].qty * ord.itm[i].price;
                        }
                        Dekh_Bill(tot);
                        found = 1;
                    }
                }
                if (!found) {
                    printf("No invoice found for %s.\n", name);
                }
                fclose(fp);
                break;

            case 4:
                printf("Exiting...\n");
                return 0;

            default:
                printf("Invalid option. Please try again.\n");
        }

        printf("\nWould you like to perform another action? [y/n]: ");
        scanf(" %c", &saveBill);
        getchar();
        if (saveBill != 'y' && saveBill != 'Y') {
            break;
        }
    }

    return 0;
}
