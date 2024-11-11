#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Недопустимое кол-во аргументов");
        return 0;
    }

    double Result = 0;

    for (int i = 0; i < argc - 1; i++)
    {
        Result += atof(argv[i + 1]);
    }

    printf("Сумма всех аргументов: %.2f\n", Result);
}