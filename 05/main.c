#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/wait.h> 
#include <signal.h>
#include <fcntl.h>

#define MAX_NUMBERS 100
#define FILE_NAME "output.txt"

int file_locked = 0;

void handle_sigusr1(int sig) {
    file_locked = 1;
}

void handle_sigusr2(int sig) {
    file_locked = 0;
}

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

        struct sigaction sa;
        sa.sa_handler = handle_sigusr2;
        sa.sa_flags = 0;
        sigemptyset(&sa.sa_mask);
        sigaction(SIGUSR2, &sa, NULL);

        srand(time(NULL)); 
        for (int i = 0; i < count; i++) {
            while (file_locked) {
                pause(); // Ждем разрешения на запись
            }

            int random_number = rand() % 100; 
            write(pipe_fd[1], &random_number, sizeof(random_number)); 

            FILE *file = fopen(FILE_NAME, "a");
            if (file == NULL) {
                perror("Ошибка открытия файла");
                return 1;
            }
            fprintf(file, "%d\n", random_number);
            fclose(file);

            kill(getppid(), SIGUSR1); // Блокировка доступа к файлу
            sleep(1); // Задержка для демонстрации синхронизации
            kill(getppid(), SIGUSR2); // Разрешение доступа к файлу

            // Ждем разрешения на чтение файла
            while (file_locked) {
                pause();
            }

            file = fopen(FILE_NAME, "r");
            if (file == NULL) {
                perror("Ошибка открытия файла");
                return 1;
            }

            int file_number;
            while (fscanf(file, "%d", &file_number) != EOF) {
                printf("Число из файла: %d\n", file_number);
            }

            fclose(file);
        }

        close(pipe_fd[1]); 
        exit(0);
    } 
    
    else { 
        close(pipe_fd[1]);

        struct sigaction sa;
        sa.sa_handler = handle_sigusr1;
        sa.sa_flags = 0;
        sigemptyset(&sa.sa_mask);
        sigaction(SIGUSR1, &sa, NULL);

        FILE *file = fopen(FILE_NAME, "w");
        if (file == NULL) {
            perror("Ошибка открытия файла");
            return 1;
        }
        fclose(file);

        int received_number;
        for (int i = 0; i < count; i++) {
            read(pipe_fd[0], &received_number, sizeof(received_number)); 
            printf("Получено число: %d\n", received_number); 

            // Блокировка доступа к файлу
            file_locked = 1;
            FILE *file = fopen(FILE_NAME, "a");
            if (file == NULL) {
                perror("Ошибка открытия файла");
                return 1;
            }
            fprintf(file, "%d\n", received_number);
            fclose(file);

            // Разрешение доступа к файлу
            file_locked = 0;
            kill(pid, SIGUSR2); // Разрешение доступа к файлу

            // Ждем разрешения на чтение файла
            while (file_locked) {
                pause();
            }
        }

        close(pipe_fd[0]); 

        wait(NULL);
    }

    return 0;
}
