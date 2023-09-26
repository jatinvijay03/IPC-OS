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
	struct message rcv_msg;
	message.mtype = id;
	while(1){
		printf("1. Enter 1 to contact the Ping Server\n2. Enter 2 to contact the File Search Server\n3. Enter 3 to contact the File Word Count Server\n4. Enter 4 if this Client wishes to exit\nYour choice: ");
		char choice;
		scanf("%s", &choice);
		strcpy(message.mtext, &choice);
		if(choice == '4'){
			break;
		}
		if(choice == '1'){
			strcpy(message.mtext, "hi");
			msgsnd(msqid, &message, sizeof(message.mtext), 0);
			msgrcv(msqid, &rcv_msg, sizeof(rcv_msg.mtext), id, 0);
			printf("Message received from server: %s\n", rcv_msg.mtext);
		}
		else if(choice == '2'){
			strcpy(message.mtext, "2");
			char filename[100];
			printf("Enter filename: ");
			scanf("%s", filename);
			strcat(message.mtext, " ");
			strcat(message.mtext, filename);
			msgsnd(msqid, &message, sizeof(message.mtext), 0);
			msgrcv(msqid, &rcv_msg, sizeof(rcv_msg.mtext), id, 0);
			// if(rcv_msg.mtext[0] == '\0')
			// 	printf("File not found\n");
			// else
			// 	printf("File found\n");
			printf("%s", rcv_msg.mtext);
		}
		else if(choice == '3'){
			strcpy(message.mtext, "3");
			char filename[100];
			printf("Enter filename: ");
			scanf("%s", filename);
			strcat(message.mtext, " ");
			strcat(message.mtext, filename);
			msgsnd(msqid, &message, sizeof(message.mtext), 0);
			msgrcv(msqid, &rcv_msg, sizeof(rcv_msg.mtext), id, 0);
			printf("No of words and FileName: %s\n", rcv_msg.mtext);
		}
		else{printf("Invalid choice\n"); continue;}
		
		
	}
	
	
	

	
    

	return 0;
}

