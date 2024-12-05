#include "message.h"

int main() {
    key_t key = ftok("queue", 65);
    int msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("msgget");
        exit(1);
    }

    Message msg;
    msg.mtype = 1;

    while (1) {
        printf("Client: Enter message (type 'END' to exit): ");
        fgets(msg.mtext, MSG_SIZE, stdin);
        msg.mtext[strcspn(msg.mtext, "\n")] = 0;

        if (strcmp(msg.mtext, "END") == 0) {
            msg.mtype = END_PRIORITY;
        }

        if (msgsnd(msgid, &msg, sizeof(msg.mtext), 0) == -1) {
            perror("msgsnd");
            exit(1);
        }

        if (msg.mtype == END_PRIORITY) {
            printf("Client: Sent end message, exiting.\n");
            break;
        }

        if (msgrcv(msgid, &msg, sizeof(msg.mtext), 1, 0) == -1) {
            perror("msgrcv");
            exit(1);
        }

        printf("Client: Received message: %s\n", msg.mtext);
    }

    msgctl(msgid, IPC_RMID, NULL);
    return 0;
}
