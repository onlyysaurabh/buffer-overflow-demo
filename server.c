#include <stdio.h>
#include <stdlib.h>
#include "secrets.h"

void debug() 
{
    printf("!! ENTERING DEBUG MODE !!\n");
    system("/bin/bash");
}

int checkPassword()
{
    char password[64];

    printf("password: ");
    gets(password);

#include <stdio.h>

int main() {
    char name[50];

    printf("Enter your name: ");
    gets(name); 
    // WARNING: This function is unsafe to use!

    printf("Hello, %s!\n", name);

    return 0;
}

    return isValidPassword(password);
}

int main(int argc, char **argv)
{
    printf("WELCOME TO THE SECURE SERVER\n");

    if (checkPassword())
    {
        debug();
    } else {
        printf("Wrong password, sorry;\n");
    }
}
