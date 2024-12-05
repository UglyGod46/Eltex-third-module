#include "message.h"

int main() {
    key_t key = ftok("queue", 65);
    int msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("msgget");
        exit(1);
    }

    Message msg;
    while (1) {
        if (msgrcv(msgid, &msg, sizeof(msg.mtext), 0, 0) == -1) {
            perror("msgrcv");
            exit(1);
        }

        if (msg.mtype == END_PRIORITY) {
            printf("Server: Received end message, exiting.\n");
            break;
        }

        printf("Server: Received message: %s\n", msg.mtext);

        // Prepare response
        snprintf(msg.mtext, MSG_SIZE, "Pong: %s", msg.mtext);
        msg.mtype = 1;

        if (msgsnd(msgid, &msg, sizeof(msg.mtext), 0) == -1) {
            perror("msgsnd");
            exit(1);
        }
    }

    msgctl(msgid, IPC_RMID, NULL);
    return 0;
}
