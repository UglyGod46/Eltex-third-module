#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void CalculateArea(double side)
{
    double Result = side * side;

    printf("Площадь квадрата со стороной %.2f = %.2f\n",side, Result);
}

int main(int argc, char *argv[])
{
    int Count_Sides = argc - 1;
    int Middle_Sides = Count_Sides / 2;

    if (argc < 2)
    {
        fprintf(stderr, "Использование: %s <side_length1> <side_length2> ...\n", argv[0]);
        return 0;
    }

    pid_t pid = fork();

    if (pid < 0)
    {
        perror("fork");
        exit(0);
    }

    else if (pid == 0)
    {
        for (int i = Middle_Sides; i < Count_Sides; i++)
        {
            double Side = atof(argv[i + 1]);
            CalculateArea(Side);
        }
    }

    else
    {
        for (int i = 0; i < Middle_Sides; i++)
        {
            double Side = atof(argv[i + 1]);
            CalculateArea(Side);
        }

        wait(NULL);
    }
    
    return 0;
}