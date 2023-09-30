#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

    int id;
    printf("Enter client id (cannot put 1000 as thwe value): ");
    scanf("%d", &id);

    struct message message;
    struct message rcv_msg;
    message.mtype = id;

    while (1) {
        printf("1. Enter 1 to contact the Ping Server\n2. Enter 2 to contact the File Search Server\n3. Enter 3 to contact the File Word Count Server\n4. Enter 4 if this Client wishes to exit\nYour choice: ");
        char choice;
        scanf(" %c", &choice); // Note the space before %c to consume newline characters.

        if (choice == '4') {
            break;
        }

        if (choice == '1') {
            strcpy(message.mtext, "hi");
            if (msgsnd(msqid, &message, sizeof(message.mtext), 0) == -1) {
                perror("msgsnd");
                exit(-3);
            }
            if (msgrcv(msqid, &rcv_msg, sizeof(rcv_msg.mtext), id, 0) == -1) {
                perror("msgrcv");
                exit(-4);
            }
            printf("Message received from server: %s\n", rcv_msg.mtext);
        } else if (choice == '2') {
            strcpy(message.mtext, "2");
            char filename[100];
            printf("Enter filename: ");
            scanf("%s", filename);
            strcat(message.mtext, " ");
            strcat(message.mtext, filename);
            if (msgsnd(msqid, &message, sizeof(message.mtext), 0) == -1) {
                perror("msgsnd");
                exit(-5);
            }
            if (msgrcv(msqid, &rcv_msg, sizeof(rcv_msg.mtext), id, 0) == -1) {
                perror("msgrcv");
                exit(-6);
            }
            printf("%s", rcv_msg.mtext);
        } else if (choice == '3') {
            strcpy(message.mtext, "3");
            char filename[100];
            printf("Enter filename: ");
            scanf("%s", filename);
            strcat(message.mtext, " ");
            strcat(message.mtext, filename);
            if (msgsnd(msqid, &message, sizeof(message.mtext), 0) == -1) {
                perror("msgsnd");
                exit(-7);
            }
            if (msgrcv(msqid, &rcv_msg, sizeof(rcv_msg.mtext), id, 0) == -1) {
                perror("msgrcv");
                exit(-8);
            }
            printf("No of words and FileName: %s\n", rcv_msg.mtext);
        } else {
            printf("Invalid choice\n");
            continue;
        }
    }

    return 0;
}
