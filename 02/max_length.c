#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Недопустимое кол-во аргументов");
        return 0;
    }

    int Max_len = 0;

    for (int i = 1; i < argc; i++)
    {
        int Current_len = strlen(argv[i]);
        if (Current_len > Max_len)
        {
            Max_len = Current_len;
        }
    }

    printf("Максимальная длина строки: %d\n", Max_len);
}