#include "header.h"

const char *RECORDS = "./data/records.txt";

// reads one record from file, returns 0 on EOF
int getAccountFromFile(FILE *ptr, char name[50], struct Record *r)
{
    return fscanf(ptr, "%d %d %s %d %d/%d/%d %s %ld %lf %s",
                  &r->id,
                  &r->userId,
                  name,
                  &r->accountNbr,
                  &r->deposit.month,
                  &r->deposit.day,
                  &r->deposit.year,
                  r->country,
                  &r->phone,
                  &r->amount,
                  r->accountType) != EOF;
}

// writes one record to file
void saveAccountToFile(FILE *ptr, struct User u, struct Record r)
{
    fprintf(ptr, "%d %d %s %d %d/%d/%d %s %ld %.2lf %s\n\n",
            r.id,
            u.id,
            u.name,
            r.accountNbr,
            r.deposit.month,
            r.deposit.day,
            r.deposit.year,
            r.country,
            r.phone,
            r.amount,
            r.accountType);
}

void stayOrReturn(int notGood, void f(struct User u), struct User u)
{
    int option;
    if (notGood == 0)
    {
        system("clear");
        printf("\n✖ Record not found!!\n");
    invalid:
        printf("\nEnter 0 to try again, 1 to return to main menu and 2 to exit:");
        scanf("%d", &option);
        if (option == 0)
            f(u);
        else if (option == 1)
            mainMenu(u);
        else if (option == 2)
            exit(0);
        else
        {
            printf("Insert a valid operation!\n");
            goto invalid;
        }
    }
    else
    {
        printf("\nEnter 1 to go to the main menu and 0 to exit:");
        scanf("%d", &option);
        if (option == 1)
        {
            system("clear");
            mainMenu(u);
        }
        else
        {
            system("clear");
            exit(1);
        }
    }
}

void success(struct User u)
{
    int option;
    printf("\n✔ Success!\n\n");
invalid:
    printf("Enter 1 to go to the main menu and 0 to exit!\n");
    scanf("%d", &option);
    system("clear");
    if (option == 1)
        mainMenu(u);
    else if (option == 0)
        exit(1);
    else
    {
        printf("Insert a valid operation!\n");
        goto invalid;
    }
}

void createNewAcc(struct User u)
{
    struct Record r;
    struct Record cr;
    char userName[50];
    FILE *pf;
    int lastId = 0;

noAccount:
    system("clear");
    printf("\t\t\t===== New record =====\n");

    printf("\nEnter today's date(mm/dd/yyyy):");
    scanf("%d/%d/%d", &r.deposit.month, &r.deposit.day, &r.deposit.year);
    printf("\nEnter the account number:");
    scanf("%d", &r.accountNbr);

    pf = fopen(RECORDS, "a+");
    // check for duplicate account number for this user
    while (getAccountFromFile(pf, userName, &cr))
    {
        if (strcmp(userName, u.name) == 0 && cr.accountNbr == r.accountNbr)
        {
            printf("✖ This Account already exists for this user\n\n");
            fclose(pf);
            goto noAccount;
        }
        if (cr.id >= lastId)
            lastId = cr.id + 1; // track next available record id
    }

    printf("\nEnter the country:");
    scanf("%s", r.country);
    printf("\nEnter the phone number:");
    scanf("%d", &r.phone);
    printf("\nEnter amount to deposit: $");
    scanf("%lf", &r.amount);
    printf("\nChoose the type of account:\n\t-> saving\n\t-> current\n\t-> fixed01(for 1 year)\n\t-> fixed02(for 2 years)\n\t-> fixed03(for 3 years)\n\n\tEnter your choice:");
    scanf("%s", r.accountType);

    r.id = lastId;
    r.userId = u.id;

    saveAccountToFile(pf, u, r);
    fclose(pf);
    success(u);
}

void checkAllAccounts(struct User u)
{
    char userName[100];
    struct Record r;

    FILE *pf = fopen(RECORDS, "r");

    system("clear");
    printf("\t\t====== All accounts from user: %s =====\n\n", u.name);
    while (getAccountFromFile(pf, userName, &r))
    {
        if (strcmp(userName, u.name) == 0)
        {
            printf("_____________________\n");
            printf("\nAccount number  : %d\nDeposit Date    : %d/%d/%d\nCountry         : %s\nPhone number    : %ld\nAmount deposited: $%.2f\nType of account : %s\n",
                   r.accountNbr,
                   r.deposit.day,
                   r.deposit.month,
                   r.deposit.year,
                   r.country,
                   r.phone,
                   r.amount,
                   r.accountType);
        }
    }
    fclose(pf);
    success(u);
}

// checkAccount: shows details of one account and calculates interest
void checkAccount(struct User u)
{
    char userName[50];
    struct Record r;
    int accNbr, found = 0;

    system("clear");
    printf("\t\t====== Check account details =====\n\n");
    printf("Enter the account number:");
    scanf("%d", &accNbr);

    FILE *pf = fopen(RECORDS, "r");
    while (getAccountFromFile(pf, userName, &r))
    {
        if (strcmp(userName, u.name) == 0 && r.accountNbr == accNbr)
        {
            found = 1;
            printf("\n_____________________\n");
            printf("\nAccount number  : %d\nDeposit Date    : %d/%d/%d\nCountry         : %s\nPhone number    : %ld\nAmount deposited: $%.2f\nType of account : %s\n",
                   r.accountNbr,
                   r.deposit.day,
                   r.deposit.month,
                   r.deposit.year,
                   r.country,
                   r.phone,
                   r.amount,
                   r.accountType);

            // display interest based on account type
            if (strcmp(r.accountType, "saving") == 0)
            {
                double interest = r.amount * 0.07 / 12;
                printf("\nYou will get $%.2f as interest on day %d of every month\n", interest, r.deposit.day);
            }
            else if (strcmp(r.accountType, "fixed01") == 0)
            {
                double interest = r.amount * 0.04;
                printf("\nYou will get $%.2f as interest after 1 year\n", interest);
            }
            else if (strcmp(r.accountType, "fixed02") == 0)
            {
                double interest = r.amount * 0.05;
                printf("\nYou will get $%.2f as interest after 2 years\n", interest);
            }
            else if (strcmp(r.accountType, "fixed03") == 0)
            {
                double interest = r.amount * 0.08;
                printf("\nYou will get $%.2f as interest after 3 years\n", interest);
            }
            else if (strcmp(r.accountType, "current") == 0)
            {
                printf("\nYou will not get interests because the account is of type current\n");
            }
            break;
        }
    }
    fclose(pf);
    stayOrReturn(found, checkAccount, u);
}

// updateAccount: allows user to update country or phone number of an account
void updateAccount(struct User u)
{
    char userName[50];
    struct Record r;
    int accNbr, found = 0, field;

    system("clear");
    printf("\t\t====== Update account =====\n\n");
    printf("Enter the account number to update:");
    scanf("%d", &accNbr);

    // read all records into a temp file, modify the matching one
    FILE *pf = fopen(RECORDS, "r");
    FILE *tmp = fopen("./data/tmp.txt", "w");

    while (getAccountFromFile(pf, userName, &r))
    {
        if (strcmp(userName, u.name) == 0 && r.accountNbr == accNbr)
        {
            found = 1;
            printf("\nWhat do you want to update?\n\t[1]- Country\n\t[2]- Phone number\n\tEnter your choice:");
            scanf("%d", &field);
            if (field == 1)
            {
                printf("Enter new country:");
                scanf("%s", r.country);
            }
            else if (field == 2)
            {
                printf("Enter new phone number:");
                scanf("%d", &r.phone);
            }
            else
            {
                printf("Invalid option!\n");
            }
        }
        // write record (updated or unchanged) to temp file
        fprintf(tmp, "%d %d %s %d %d/%d/%d %s %ld %.2lf %s\n\n",
                r.id, r.userId, userName,
                r.accountNbr,
                r.deposit.month, r.deposit.day, r.deposit.year,
                r.country, r.phone, r.amount, r.accountType);
    }
    fclose(pf);
    fclose(tmp);

    // replace records file with updated temp file
    remove(RECORDS);
    rename("./data/tmp.txt", RECORDS);

    stayOrReturn(found, updateAccount, u);
}

// makeTransaction: deposit or withdraw from an account (fixed accounts not allowed)
void makeTransaction(struct User u)
{
    char userName[50];
    struct Record r;
    int accNbr, found = 0, type;
    double amount;

    system("clear");
    printf("\t\t====== Make transaction =====\n\n");
    printf("Enter the account number:");
    scanf("%d", &accNbr);

    FILE *pf = fopen(RECORDS, "r");
    FILE *tmp = fopen("./data/tmp.txt", "w");

    while (getAccountFromFile(pf, userName, &r))
    {
        if (strcmp(userName, u.name) == 0 && r.accountNbr == accNbr)
        {
            found = 1;
            // fixed accounts cannot make transactions
            if (strcmp(r.accountType, "fixed01") == 0 ||
                strcmp(r.accountType, "fixed02") == 0 ||
                strcmp(r.accountType, "fixed03") == 0)
            {
                printf("\n✖ You can not make transactions on a fixed account!\n");
            }
            else
            {
                printf("\n[1]- Deposit\n[2]- Withdraw\nEnter your choice:");
                scanf("%d", &type);
                printf("Enter amount: $");
                scanf("%lf", &amount);
                if (type == 1)
                {
                    r.amount += amount;
                }
                else if (type == 2)
                {
                    if (amount > r.amount)
                        printf("\n✖ Insufficient funds!\n");
                    else
                        r.amount -= amount;
                }
                printf("\nNew balance: $%.2f\n", r.amount);
            }
        }
        fprintf(tmp, "%d %d %s %d %d/%d/%d %s %ld %.2lf %s\n\n",
                r.id, r.userId, userName,
                r.accountNbr,
                r.deposit.month, r.deposit.day, r.deposit.year,
                r.country, r.phone, r.amount, r.accountType);
    }
    fclose(pf);
    fclose(tmp);

    remove(RECORDS);
    rename("./data/tmp.txt", RECORDS);

    stayOrReturn(found, makeTransaction, u);
}

// removeAccount: deletes an account owned by the user
void removeAccount(struct User u)
{
    char userName[50];
    struct Record r;
    int accNbr, found = 0;

    system("clear");
    printf("\t\t====== Remove account =====\n\n");
    printf("Enter the account number to remove:");
    scanf("%d", &accNbr);

    FILE *pf = fopen(RECORDS, "r");
    FILE *tmp = fopen("./data/tmp.txt", "w");

    while (getAccountFromFile(pf, userName, &r))
    {
        // skip (do not write) the account to be removed
        if (strcmp(userName, u.name) == 0 && r.accountNbr == accNbr)
        {
            found = 1;
            continue;
        }
        fprintf(tmp, "%d %d %s %d %d/%d/%d %s %ld %.2lf %s\n\n",
                r.id, r.userId, userName,
                r.accountNbr,
                r.deposit.month, r.deposit.day, r.deposit.year,
                r.country, r.phone, r.amount, r.accountType);
    }
    fclose(pf);
    fclose(tmp);

    remove(RECORDS);
    rename("./data/tmp.txt", RECORDS);

    stayOrReturn(found, removeAccount, u);
}

// transferOwner: transfers an account to another user
void transferOwner(struct User u)
{
    char userName[50], newOwner[50];
    struct Record r;
    int accNbr, found = 0;

    system("clear");
    printf("\t\t====== Transfer account ownership =====\n\n");
    printf("Enter the account number to transfer:");
    scanf("%d", &accNbr);
    printf("Enter the username of the new owner:");
    scanf("%s", newOwner);

    // verify new owner exists in users.txt
    FILE *uf = fopen("./data/users.txt", "r");
    struct User tmp;
    int ownerFound = 0;
    while (fscanf(uf, "%d %s %s", &tmp.id, tmp.name, tmp.password) != EOF)
    {
        if (strcmp(tmp.name, newOwner) == 0)
        {
            ownerFound = 1;
            break;
        }
    }
    fclose(uf);

    if (!ownerFound)
    {
        printf("\n✖ User '%s' not found!\n", newOwner);
        stayOrReturn(0, transferOwner, u);
        return;
    }

    FILE *pf = fopen(RECORDS, "r");
    FILE *tf = fopen("./data/tmp.txt", "w");

    while (getAccountFromFile(pf, userName, &r))
    {
        if (strcmp(userName, u.name) == 0 && r.accountNbr == accNbr)
        {
            found = 1;
            // change owner to new user
            r.userId = tmp.id;
            fprintf(tf, "%d %d %s %d %d/%d/%d %s %ld %.2lf %s\n\n",
                    r.id, tmp.id, newOwner,
                    r.accountNbr,
                    r.deposit.month, r.deposit.day, r.deposit.year,
                    r.country, r.phone, r.amount, r.accountType);
        }
        else
        {
            fprintf(tf, "%d %d %s %d %d/%d/%d %s %ld %.2lf %s\n\n",
                    r.id, r.userId, userName,
                    r.accountNbr,
                    r.deposit.month, r.deposit.day, r.deposit.year,
                    r.country, r.phone, r.amount, r.accountType);
        }
    }
    fclose(pf);
    fclose(tf);

    remove(RECORDS);
    rename("./data/tmp.txt", RECORDS);

    stayOrReturn(found, transferOwner, u);
}
