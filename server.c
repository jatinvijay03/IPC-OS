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
	int msqid = msgget(key, 0666 | IPC_CREAT);

	struct message rcv_msg;
	struct message snd_msg;
	
	while(1){
		msgrcv(msqid, &rcv_msg, sizeof(rcv_msg.mtext), 0, 0);
		snd_msg.mtype = rcv_msg.mtype;
		int fd[2];
		pipe(fd);
		int isParent = fork();
		if(isParent){
			close(fd[0]);
			write(fd[1], rcv_msg.mtext, sizeof(rcv_msg.mtext));
			close(fd[1]);
			continue;
		}



		else{
			char prompt[100];
			close(fd[1]);
			read(fd[0], prompt, sizeof(prompt));
			close(fd[0]);
			if(strcmp("hi",prompt) == 0){
				
				strcpy(snd_msg.mtext, "hello");
				
				exit(0);
				}
			else if (prompt[0] == '2')
			{
				char command[100] = "find . -name ";
				memmove(prompt, prompt+2, strlen(prompt));
				strcat(command, prompt);
				strcpy(snd_msg.mtext, command);
				
				execl("/bin/sh", "sh", "-c", command, NULL);
			}
			else if (prompt[0] == '3')
			{	
				char command[100] = "wc -w ";
				memmove(prompt, prompt+2, strlen(prompt));
				strcat(command, prompt);
				strcpy(snd_msg.mtext, command);
				execl("/bin/sh", "sh", "-c", command, NULL);
				
				exit(0);
			}
			msgsnd(msqid, &snd_msg, sizeof(snd_msg.mtext), 0);
			
			}
		
	}
	
    while(1);

	return 0;
}
