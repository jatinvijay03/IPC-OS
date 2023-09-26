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
		int isParent = fork();
		if(isParent){
			continue;
		}


		else{
			if(strcmp("hi",rcv_msg.mtext) == 0){
				
				strcpy(snd_msg.mtext, "hello");
				msgsnd(msqid, &snd_msg, sizeof(snd_msg.mtext), 0);
				exit(0);
				}
			else if (rcv_msg.mtext[0] == '2')
			{
				char command[100] = "find . -name ";
				memmove(rcv_msg.mtext, rcv_msg.mtext+2, strlen(rcv_msg.mtext));
				strcat(command, rcv_msg.mtext);
				strcpy(snd_msg.mtext, system(command));
				msgsnd(msqid, &snd_msg, sizeof(snd_msg.mtext), 0);
				exit(0);
			}
			else if (rcv_msg.mtext[0] == '3')
			{	
				char command[100] = "wc -w ";
				memmove(rcv_msg.mtext, rcv_msg.mtext+2, strlen(rcv_msg.mtext));
				strcat(command, rcv_msg.mtext);
				strcpy(snd_msg.mtext, system(command));
				msgsnd(msqid, &snd_msg, sizeof(snd_msg.mtext), 0);
				exit(0);
			}
			
			}
		
	}
	
    while(1);

	return 0;
}
