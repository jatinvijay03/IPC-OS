#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>

struct message {
	long mtype;
	char mtext[100];
};

int main() {	
	key_t key = ftok("path_to_key_file", 65);
	int msqid = msgget(key, 0666 | IPC_CREAT);

	struct message rcv_msg;
	while(1){
		msgrcv(msqid, &rcv_msg, sizeof(rcv_msg.mtext), 0, 0);

		printf("Received message: %s\n", rcv_msg.mtext);
	}
	
    while(1);

	return 0;
}
