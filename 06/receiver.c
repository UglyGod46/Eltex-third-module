#include "personchat.h"

int main() {
    key_t key = ftok("chat_keyfile", 65); // Генерация ключа
    if (key == -1) {
        perror("ftok");
        exit(1);
    }

    int msgid = msgget(key, 0666 | IPC_CREAT); // Создание очереди сообщений
    if (msgid == -1) {
        perror("msgget");
        exit(1);
    }

    Message msg;

    while (1) {
        if (msgrcv(msgid, &msg, sizeof(Message) - sizeof(long), MSG_TYPE_PING, 0) == -1) {
            perror("msgrcv");
            exit(1);
        }

        if (msg.mtype == MSG_TYPE_QUIT) {
            break;
        }

        printf("Получено сообщение: %s\n", msg.mtext);

        msg.mtype = MSG_TYPE_PONG;
        strcpy(msg.mtext, "PONG");

        if (msgsnd(msgid, &msg, sizeof(Message) - sizeof(long), 0) == -1) {
            perror("msgsnd");
            exit(1);
        }
    }

    if (msgctl(msgid, IPC_RMID, NULL) == -1) {
        perror("msgctl");
        exit(1);
    }

    return 0;
}