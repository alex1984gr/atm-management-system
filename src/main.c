#include "header.h"

void mainMenu(struct User u)
{
    int option;
    system("clear");
    printf("\n\n\t\t======= ATM =======\n\n");
    printf("\n\t\t-->> Feel free to choose one of the options below <<--\n");
    printf("\n\t\t[1]- Create a new account\n");
    printf("\n\t\t[2]- Update account information\n");
    printf("\n\t\t[3]- Check accounts\n");
    printf("\n\t\t[4]- Check list of owned account\n");
    printf("\n\t\t[5]- Make Transaction\n");
    printf("\n\t\t[6]- Remove existing account\n");
    printf("\n\t\t[7]- Transfer ownership\n");
    printf("\n\t\t[8]- Exit\n");
    while (!readInt(&option))
        printf("Insert a valid number: ");

    switch (option)
    {
    case 1:
        createNewAcc(u);
        break;
    case 2:
        updateAccount(u);
        break;
    case 3:
        checkAccount(u);
        break;
    case 4:
        checkAllAccounts(u);
        break;
    case 5:
        makeTransaction(u);
        break;
    case 6:
        removeAccount(u);
        break;
    case 7:
        transferOwner(u);
        break;
    case 8:
        exit(0);
        break;
    default:
        printf("Invalid operation!\n");
        mainMenu(u);
    }
}

void initMenu(struct User *u)
{
    int r = 0;
    int option;
    system("clear");
    printf("\n\n\t\t======= ATM =======\n");
    printf("\n\t\t-->> Feel free to login / register :\n");
    printf("\n\t\t[1]- login\n");
    printf("\n\t\t[2]- register\n");
    printf("\n\t\t[3]- exit\n");
    while (!r)
    {
        if (!readInt(&option))
        {
            printf("Insert a valid number!\n");
            continue;
        }
        switch (option)
        {
        case 1:
            loginMenu(u->name, u->password);
            // verify password matches stored password
            if (strcmp(u->password, getPassword(*u)) == 0)
            {
                // load user id from users.txt
                FILE *fp = fopen("./data/users.txt", "r");
                struct User tmp;
                while (fscanf(fp, "%d %s %s", &tmp.id, tmp.name, tmp.password) != EOF)
                {
                    if (strcmp(tmp.name, u->name) == 0)
                    {
                        u->id = tmp.id;
                        break;
                    }
                }
                fclose(fp);
                printf("\n\nPassword Match! Welcome, %s\n", u->name);
            }
            else
            {
                printf("\nWrong password or username!\n");
                exit(1);
            }
            r = 1;
            break;
        case 2:
            registerMenu(u);
            r = 1;
            break;
        case 3:
            exit(0);
            break;
        default:
            printf("Insert a valid operation!\n");
        }
    }
}

int main()
{
    struct User u;

    initMenu(&u);
    mainMenu(u);
    return 0;
}
