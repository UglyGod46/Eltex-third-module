#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Недопустимое кол-во аргументов");
        return 0;
    }

    double Max_arg = 0;

    for (int i = 0; i < argc - 1; i++)
    {
        double Current_arg = atof(argv[i + 1]);

        if (Current_arg > Max_arg)
        {
            Max_arg = Current_arg;
        }
    }
    printf("Максимальный аргумент: %.2f\n", Max_arg);
}