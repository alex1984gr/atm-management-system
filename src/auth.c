#include "header.h"

char *USERS = "./data/users.txt";

void loginMenu(char a[50], char pass[50])
{
    system("clear");
    printf("\n\n\n\t\t\t\t   Bank Management System\n\t\t\t\t\t User Login:");
    scanf("%s", a);
    printf("\n\n\n\n\n\t\t\t\tEnter the password to login:");
    scanf("%s", pass);
}

// registerMenu: reads new username/password, checks uniqueness, saves to users.txt
void registerMenu(struct User *u)
{
    FILE *fp;
    struct User tmp;
    int id = 0;

    printf("\n\n\t\t\t\tEnter a new username:");
    scanf("%s", u->name);

    // check if username already exists
    fp = fopen("./data/users.txt", "r");
    if (fp != NULL)
    {
        while (fscanf(fp, "%d %s %s", &tmp.id, tmp.name, tmp.password) != EOF)
        {
            if (strcmp(tmp.name, u->name) == 0)
            {
                printf("\n✖ Username already exists! Please choose another.\n");
                fclose(fp);
                registerMenu(u);
                return;
            }
            id = tmp.id + 1; // track next available id
        }
        fclose(fp);
    }

    printf("\n\t\t\t\tEnter a password:");
    scanf("%s", u->password);

    u->id = id;

    // append new user to users.txt
    fp = fopen("./data/users.txt", "a");
    if (fp == NULL)
    {
        printf("Error opening users file!\n");
        exit(1);
    }
    fprintf(fp, "%d %s %s\n", u->id, u->name, u->password);
    fclose(fp);

    printf("\n✔ Registration successful! Welcome, %s\n", u->name);
}

// getPassword: looks up the password for the given user in users.txt
const char *getPassword(struct User u)
{
    FILE *fp;
    struct User userChecker;
    static char buff[50]; // static so it persists after return

    fp = fopen("./data/users.txt", "r");
    if (fp == NULL)
    {
        printf("Error! opening file");
        exit(1);
    }

    while (fscanf(fp, "%d %s %s", &userChecker.id, userChecker.name, userChecker.password) != EOF)
    {
        if (strcmp(userChecker.name, u.name) == 0)
        {
            fclose(fp);
            strcpy(buff, userChecker.password);
            return buff;
        }
    }

    fclose(fp);
    return "no user found";
}
