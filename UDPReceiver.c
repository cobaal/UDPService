#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <signal.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]) {

	// ARGV
	if (argc != 3) {
		printf("PLEASE INPUT IP & PORT#\n");
		return 0;
	}

	// Make UDP Socket Discriptor
	int serverSocket;
	printf("RECEIVER : Creating UDP Socket...\n");
	if ((serverSocket = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("Socket Fail");
		exit(1);
	}

	// Bind
	struct sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(struct sockaddr));
	server_addr.sin_family = AF_INET;			// IPv4 Internet Protocol
	server_addr.sin_port = htons(atoi(argv[2]));		// Port #
	server_addr.sin_addr.s_addr = inet_addr(argv[1]);	// IPv4 Address

	printf("RECEIVER : Binding...\n");
	if (bind(serverSocket, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) < 0) {
		perror("Bind Fail");
		exit(1);
	} else {
		printf("RECEIVER : Success Binding!\n");
	}

	// Fork
	int pid;
	
	if ((pid = fork()) < 0) {
		perror("fork fail");
		exit(1);
	}

	// Child Process
	if (pid == 0) {
		printf("RECEIVER PID : %d\n", getpid());

		// recv
		struct sockaddr_in client_addr;
		int client_addr_size;
		int nbyte;
		int BUFSIZE = 512;
		char buf_rcv[BUFSIZ];

		printf("RECEIVER : Waiting Request. [(Q)uit]\n");
		while(1) {
			nbyte = recvfrom(serverSocket, buf_rcv, BUFSIZE, 0, (struct sockaddr *)&client_addr, &client_addr_size);
			if (nbyte < 0) {
				perror("recvfrom fail");
				exit(1);
			}

			if (buf_rcv[0] != 0x2f)
				printf("%s] %s\n", inet_ntoa(client_addr.sin_addr), buf_rcv);
			else 
				printf("%s] 0x%x 0x%x 0x%x\n", inet_ntoa(client_addr.sin_addr), buf_rcv[0], buf_rcv[1], buf_rcv[2]);
		}

	// Parent Process
	} else {
		char exitMsg[5];
		while(1){
			scanf("%s", exitMsg);

			// close process
			if (strcmp(exitMsg, "Q") == 0) {
				if (!kill(pid, SIGINT)) {
					perror("Close Recv Process");
				}
				return 0;
			}
			
		}
	}

	return 0;
}
