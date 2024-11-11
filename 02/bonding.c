#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Недопустимое кол-во аргументов\n");
        return 0;
    }

    char String[512] = "";
    
    for (int i = 1; i < argc; i++)
    {
        strncat(String, argv[i], sizeof(String) - strlen(String) - 1);
    }

    printf("Склеенные строки: %s\n", String);
    return 0;
}