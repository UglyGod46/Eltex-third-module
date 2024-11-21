#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/wait.h> 

#define MAX_NUMBERS 100

int main(int argc, char *argv[]) {
    int pipe_fd[2]; 
    pid_t pid;
    int count;

    if (argc != 2) {
        fprintf(stderr, "Использование: %s <количество чисел>\n", argv[0]);
        return 1;
    }

    count = atoi(argv[1]);
    if (count <= 0 || count > MAX_NUMBERS) {
        fprintf(stderr, "Количество чисел должно быть от 1 до %d.\n", MAX_NUMBERS);
        return 1;
    }

    if (pipe(pipe_fd) == -1) {
        perror("Ошибка создания канала");
        return 1;
    }

    pid = fork();
    if (pid < 0) {
        perror("Ошибка создания процесса");
        return 1;
    }

    if (pid == 0) {
        close(pipe_fd[0]);

        srand(time(NULL)); 
        for (int i = 0; i < count; i++) {
            int random_number = rand() % 100; 
            write(pipe_fd[1], &random_number, sizeof(random_number)); 
        }

        close(pipe_fd[1]); 
        exit(0);
    } 
    
    else { 
        close(pipe_fd[1]);

        FILE *file = fopen("output.txt", "w");
        if (file == NULL) {
            perror("Ошибка открытия файла");
            return 1;
        }

        int received_number;
        for (int i = 0; i < count; i++) {
            read(pipe_fd[0], &received_number, sizeof(received_number)); 
            printf("Получено число: %d\n", received_number); 
            fprintf(file, "%d\n", received_number); 
        }

        fclose(file);
        close(pipe_fd[0]); 

        wait(NULL);
    }

    return 0;
}
