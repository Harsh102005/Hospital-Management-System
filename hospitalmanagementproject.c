#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

struct items {
    char item[20];
    float price;
    int qty;
    time_t timestamp;
};

struct patient {
    char name[50];
    int age;
    char disease[50];
    char city[30];
    char mobile[91];
    char doctor[50];
};

struct order {
    struct patient pat;
    char date[50];
    int numofItems;
    struct items itm[50];
};

// Function to get the current date
void getCurrentDate(char *date) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(date, "%02d-%02d-%d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
}

void BillBana(struct patient pat, char date[30]) {
    printf("\n\n");
    printf("\t\t\tHospital Management System\t\t\t");
    printf("\n----------------------------------------------------------------------------");
    printf("\nDate: %s", date);
    printf("\nInvoice to: %s", pat.name);
    printf("\nAge: %d", pat.age);
    printf("\nDisease: %s", pat.disease);
    printf("\nCity: %s", pat.city);
    printf("\nMobile: %s", pat.mobile);
    printf("\nDoctor: %s", pat.doctor);
    printf("\n----------------------------------------------------------------------------");
    printf("\nItem\t\tQuantity \tTotal \t\t\tDate/Time");
    printf("\n----------------------------------------------------------------------------");
}

void Kaisa_Banau_Bill(char item[20], int qty, float price, time_t timestamp) {
    char time_str[50];
    struct tm *tm_info;

    tm_info = localtime(&timestamp);
    strftime(time_str, 50, "       %d-%m-%Y %H:%M:%S", tm_info);

    printf("\n%-15s %-10d      %-10.2f %s", item, qty, qty*price, time_str);
}

void Dekh_Bill(float total) {
    float dis = 0.1 * total;
    float nettotal = total - dis;
    float gst = 0.12 * nettotal;
    float grandtotal = nettotal + gst;

    printf("\n----------------------------------------------------------------------------");
    printf("\nSub Total: \t\t%.2f", total);
    printf("\nDiscount @10%%:      \t%.2f", dis);
    printf("\nNet Total: \t\t%.2f", nettotal);
    printf("\nGST @12%%: \t\t%.2f", gst);
    printf("\n----------------------------------------------------------------------------");
    printf("\nGrand Total: \t\t%.2f", grandtotal);
    printf("\n----------------------------------------------------------------------------");
}

int main() {
    int opt, n;
    char saveBill;
    struct order ord;
    FILE *fp;
    char input_name[50];

    while (1) {
        printf("\n==============================Hospital Management=============================");
        printf("\n 1. Generate Invoice");
        printf("\n 2. Show all invoices");
        printf("\n 3. Search Invoice");
        printf("\n 4. Exit");
        printf("\n\n Your Choice: ");
        scanf("%d", &opt);
        fgetc(stdin);

        switch (opt) {
            case 1:
                printf("\nEnter the name of the patient: ");
                fgets(ord.pat.name, 50, stdin);
                ord.pat.name[strlen(ord.pat.name) - 1] = '\0';

                printf("Enter the age of the patient: ");
                scanf("%d", &ord.pat.age);
                fgetc(stdin);

                printf("Enter the disease: ");
                fgets(ord.pat.disease, 50, stdin);
                ord.pat.disease[strlen(ord.pat.disease) - 1] = '\0';

                printf("Enter the city: ");
                fgets(ord.pat.city, 30, stdin);
                ord.pat.city[strlen(ord.pat.city) - 1] = '\0';

                printf("Enter the mobile number: ");
                fgets(ord.pat.mobile, 15, stdin);
                ord.pat.mobile[strlen(ord.pat.mobile) - 1] = '\0';

                printf("Enter the Doctor's name appointed for the patient: ");
                fgets(ord.pat.doctor, 50, stdin);
                ord.pat.doctor[strlen(ord.pat.doctor) - 1] = '\0';

                getCurrentDate(ord.date);

                printf("\nEnter the number of treatment items: ");
                scanf("%d", &n);
                ord.numofItems = n;

                float total = 0;
                for (int i = 0; i < n; ++i) {
                    fgetc(stdin);
                    printf("Enter the name of treatment/item: ");
                    fgets(ord.itm[i].item, 20, stdin);
                    ord.itm[i].item[strlen(ord.itm[i].item) - 1] = '\0';

                    printf("Enter the quantity: ");
                    scanf("%d", &ord.itm[i].qty);

                    printf("Enter the price: ");
                    scanf("%f", &ord.itm[i].price);

                    ord.itm[i].timestamp = time(NULL);
                    total += ord.itm[i].qty * ord.itm[i].price;
                }

                BillBana(ord.pat, ord.date);
                for (int i = 0; i < ord.numofItems; ++i) {
                    Kaisa_Banau_Bill(ord.itm[i].item, ord.itm[i].qty, ord.itm[i].price, ord.itm[i].timestamp);
                }
                Dekh_Bill(total);

                printf("\nDo you want to save the invoice [y/n]: ");
                scanf(" %c", &saveBill);

                if (saveBill == 'y') {
                    fp = fopen("Hospital_Management_Invoices.txt", "a+");
                    if (fp == NULL) {
                        printf("\nError opening file.");
                        break;
                    }
                    if (fwrite(&ord, sizeof(struct order), 1, fp) != 0) {
                        printf("\nInvoice saved successfully.");
                    } else {
                        printf("\nError saving invoice.");
                    }
                    fclose(fp);
                }
                break;

            case 2:
                fp = fopen("Hospital_Management_Invoices.txt", "r");
                if (fp == NULL) {
                    printf("\nError opening file.");
                    break;
                }
                printf("\n******************** Previous Invoices ********************");
                while (fread(&ord, sizeof(struct order), 1, fp)) {
                    float tot = 0;
                    BillBana(ord.pat, ord.date);
                    for (int i = 0; i < ord.numofItems; i++) {
                        Kaisa_Banau_Bill(ord.itm[i].item, ord.itm[i].qty, ord.itm[i].price, ord.itm[i].timestamp);
                        tot += ord.itm[i].qty * ord.itm[i].price;
                    }
                    Dekh_Bill(tot);
                }
                fclose(fp);
                break;

            case 3:
                printf("\nEnter the name of the patient to search: ");
                fgets(input_name, 50, stdin);
                input_name[strlen(input_name) - 1] = '\0';

                fp = fopen("Hospital_Management_Invoices.txt", "r");
                if (fp == NULL) {
                    printf("\nError opening file.");
                    break;
                }
                printf("\n**************************** Invoice for %s ****************************", input_name);
                int found = 0;
                while (fread(&ord, sizeof(struct order), 1, fp)) {
                    float tot = 0;
                    if (strcmp(ord.pat.name, input_name) == 0) {
                        BillBana(ord.pat, ord.date);
                        for (int i = 0; i < ord.numofItems; i++) {
                            Kaisa_Banau_Bill(ord.itm[i].item, ord.itm[i].qty, ord.itm[i].price, ord.itm[i].timestamp);
                            tot += ord.itm[i].qty * ord.itm[i].price;
                        }
                        Dekh_Bill(tot);
                        found = 1;
                        break;
                    }
                }
                if (!found) {
                    printf("\nNo invoice found for the patient.");
                }
                fclose(fp);
                break;

            case 4:
                printf("\nExiting...");
                exit(0);

            default:
                printf("\nInvalid choice. Please try again.");
                break;
        }

        printf("\nWould you like to perform another action? [y/n]: ");
        scanf(" %c", &saveBill);
        if (saveBill != 'y') {
            break;
        }
    }
    return 0;
}