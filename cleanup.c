#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

struct message {
    long mtype;
    char mtext[100];
};

int main() {
    key_t key = ftok("progfile", 65);
    // if (key == -1) {
    //     perror("ftok");
    //     exit(-1);
    // }

    int msqid = msgget(key, 0666 | IPC_CREAT);
    if (msqid == -1) {
        perror("msgget");
        exit(-2);
    }

    struct message rcv_msg;
    struct message snd_msg;

    while (1) {
        printf("Do you want the server to terminate? Press Y for Yes and N for No: ");
        char choice;
        scanf(" %c", &choice);

        if (choice == 'Y' || choice == 'y') {
            struct message msg;
            msg.mtype = -1;
            strcpy(msg.mtext, "Terminate");
            if (msgsnd(msqid, &msg, sizeof(msg.mtext), 0) == -1) {
                perror("msgsnd");
                exit(-3);
            }
            exit(0);
        } else if (choice == 'N' || choice == 'n') {
            
        } else {
        
        }
    }
    return 0;
}
