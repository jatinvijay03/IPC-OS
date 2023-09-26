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

	int id;
	printf("Enter client id: ");
	scanf("%d", &id);
	struct message message;
	message.mtype = id;
	while(1){
		printf("1. Enter 1 to contact the Ping Server\n2. Enter 2 to contact the File Search Server\n3. Enter 3 to contact the File Word Count Server\n4. Enter 4 if this Client wishes to exit\nYour choice: ");
		char choice;
		scanf("%s", &choice);
		strcpy(message.mtext, &choice);
		if(choice == '4'){
			break;
		}
		msgsnd(msqid, &message, sizeof(message.mtext), 0);
		
	}
	
	
	

	
    

	return 0;
}

