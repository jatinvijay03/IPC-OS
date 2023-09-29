#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

struct message {
	long mtype;
	char mtext[100];
};

int main() {	
	key_t key = ftok("path_to_key_file", 65);
	if (key == -1) {
        perror("ftok");
        exit(-1);
    }
    int msqid = msgget(key, 0666 | IPC_CREAT);
    if (msqid == -1) {
        perror("msgget");
        exit(-2);
    }

	struct message rcv_msg;
	struct message snd_msg;
	
	while (1) {
        if (msgrcv(msqid, &rcv_msg, sizeof(rcv_msg.mtext), 0, 0) == -1) {
            perror("msgrcv");
            exit(-3);
        }

        if (strcmp(rcv_msg.mtext, "Terminate") == 0) {
            while (wait(NULL) > 0);
            if (msgctl(msqid, IPC_RMID, NULL) == -1) {
                perror("msgctl");
                exit(-4);
            }
            break;
        }

        snd_msg.mtype = rcv_msg.mtype;
        int fd1[2];
        int fd2[2];
        
        if (pipe(fd1) == -1 || pipe(fd2) == -1) {
            perror("pipe");
            exit(-5);
        }

        int isParent = fork();
        if (isParent) {
            close(fd1[0]);
            write(fd1[1], rcv_msg.mtext, sizeof(rcv_msg.mtext));
            close(fd1[1]);
            wait(NULL);
            close(fd2[1]);
            if (read(fd2[0], snd_msg.mtext, sizeof(snd_msg.mtext)) == -1) {
                perror("read");
                exit(-6);
            }
            close(fd2[0]);
            if (msgsnd(msqid, &snd_msg, sizeof(snd_msg.mtext), 0) == -1) {
                perror("msgsnd");
                exit(-7);
            }
        } else {
            char prompt[100];
            close(fd1[1]);
            if (read(fd1[0], prompt, sizeof(prompt)) == -1) {
                perror("read");
                exit(-8);
            }
            close(fd1[0]);
            
            if (strcmp("hi", prompt) == 0) {
                close(fd2[0]);
                if (write(fd2[1], "hello", sizeof("hello")) == -1) {
                    perror("write");
                    exit(-9);
                }
                close(fd2[1]);
                exit(0);
            } else if (prompt[0] == '2') {
                char command[100] = "find . -name ";
                memmove(prompt, prompt + 2, strlen(prompt));
                strcat(command, prompt);
                strcpy(snd_msg.mtext, command);
                dup2(fd2[1], STDOUT_FILENO);
                close(fd2[0]);
                close(fd2[1]);
                execl("/bin/sh", "sh", "-c", command, NULL);
                perror("execl");
                exit(-10);
            } else if (prompt[0] == '3') {
                char command[100] = "wc -w ";
                memmove(prompt, prompt + 2, strlen(prompt));
                strcat(command, prompt);
                strcpy(snd_msg.mtext, command);
                dup2(fd2[1], STDOUT_FILENO);
                close(fd2[0]);
                close(fd2[1]);
                execl("/bin/sh", "sh", "-c", command, NULL);
                perror("execl");
                exit(-11);
            }
        }
    }

    return 0;
}
