#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/wait.h> 
#include <signal.h>
#include <fcntl.h>
#include <sys/sem.h>
#include <sys/ipc.h>

#define MAX_NUMBERS 100
#define FILE_NAME "output.txt"
#define SEM_KEY 12345

int sem_id;

struct sembuf lock_write_op = {0, -1, 0}; // Операция блокировки записи
struct sembuf unlock_write_op = {0, 1, 0}; // Операция разблокировки записи
struct sembuf lock_read_op = {1, -1, 0}; // Операция блокировки чтения
struct sembuf unlock_read_op = {1, 1, 0}; // Операция разблокировки чтения

void handle_sigusr1(int sig) {
    // Обработчик сигнала для записи
}

void handle_sigusr2(int sig) {
    // Обработчик сигнала для чтения
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

    // Проверка и удаление существующего семафора
    sem_id = semget(SEM_KEY, 0, 0666);
    if (sem_id != -1) {
        if (semctl(sem_id, 0, IPC_RMID, 0) == -1) {
            perror("Ошибка удаления существующего семафора");
            return 1;
        }
    }

    // Создание нового семафора
    sem_id = semget(SEM_KEY, 2, 0666 | IPC_CREAT);
    if (sem_id == -1) {
        perror("Ошибка создания семафора");
        return 1;
    }

    // Инициализация семафоров
    union semun {
        int val;
        struct semid_ds *buf;
        unsigned short *array;
    } arg;
    arg.array = (unsigned short *)malloc(2 * sizeof(unsigned short));
    if (arg.array == NULL) {
        perror("Ошибка выделения памяти");
        return 1;
    }
    arg.array[0] = 1; // Начальное значение семафора записи
    arg.array[1] = 1; // Начальное значение семафора чтения
    if (semctl(sem_id, 0, SETALL, arg) == -1) {
        perror("Ошибка инициализации семафора");
        return 1;
    }
    free(arg.array);

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
            // Блокировка доступа к файлу для записи
            semop(sem_id, &lock_write_op, 1);

            int random_number = rand() % 100; 
            write(pipe_fd[1], &random_number, sizeof(random_number)); 

            FILE *file = fopen(FILE_NAME, "a");
            if (file == NULL) {
                perror("Ошибка открытия файла");
                return 1;
            }
            fprintf(file, "%d\n", random_number);
            fclose(file);

            // Разблокировка доступа к файлу для записи
            semop(sem_id, &unlock_write_op, 1);

            kill(getppid(), SIGUSR1); // Блокировка доступа к файлу
            sleep(1); // Задержка для демонстрации синхронизации
            kill(getppid(), SIGUSR2); // Разрешение доступа к файлу

            // Ждем разрешения на чтение файла
            semop(sem_id, &lock_read_op, 1);

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

            // Разблокировка доступа к файлу для чтения
            semop(sem_id, &unlock_read_op, 1);
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

            // Блокировка доступа к файлу для записи
            semop(sem_id, &lock_write_op, 1);

            file = fopen(FILE_NAME, "a");
            if (file == NULL) {
                perror("Ошибка открытия файла");
                return 1;
            }
            fprintf(file, "%d\n", received_number);
            fclose(file);

            // Разблокировка доступа к файлу для записи
            semop(sem_id, &unlock_write_op, 1);

            kill(pid, SIGUSR2); // Разрешение доступа к файлу

            // Ждем разрешения на чтение файла
            semop(sem_id, &lock_read_op, 1);

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

            // Разблокировка доступа к файлу для чтения
            semop(sem_id, &unlock_read_op, 1);
        }

        close(pipe_fd[0]); 

        wait(NULL);

        // Удаление семафора
        if (semctl(sem_id, 0, IPC_RMID, 0) == -1) {
            perror("Ошибка удаления семафора");
            return 1;
        }
    }

    return 0;
}
