#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_MSG_SIZE 1024
#define MSG_TYPE_PING 1
#define MSG_TYPE_PONG 2
#define MSG_TYPE_QUIT 3

typedef struct {
    long mtype; // Тип сообщения
    char mtext[MAX_MSG_SIZE]; // Текст сообщения
} Message;
