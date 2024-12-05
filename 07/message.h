#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>
#include <unistd.h>

#define MSG_SIZE 1024
#define END_PRIORITY 10

typedef struct {
    long mtype;
    char mtext[MSG_SIZE];
} Message;
