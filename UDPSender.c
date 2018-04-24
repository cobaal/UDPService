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

	char msg[512];
	int BUFSIZE = 512;
	char buf_msg[BUFSIZ];

	// Make UDP Socket Discriptor
	int serverSocket;
	printf("SENDER : Creating UDP Socket...\n");
	if ((serverSocket = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("Socket Fail");
		exit(1);
	}
		
	// Make destination addr
	struct sockaddr_in d_addr;
	memset(&d_addr, 0, sizeof(struct sockaddr));
	d_addr.sin_family = AF_INET;			// IPv4 Internet Protocol
	d_addr.sin_port = htons(atoi(argv[2]));		// Port #
	d_addr.sin_addr.s_addr = inet_addr(argv[1]);	// IPv4 Address		

	int pid = -1;

	while(1) {
		// Get destination Addr
		printf("%s:%s] ", argv[1], argv[2]);
		scanf("%s", buf_msg);

		// Exit process
		if (strcmp(buf_msg, "exit") == 0 && pid == -1) 
			return 0;

		else if (strcmp(buf_msg, "exit") == 0 && pid != -1) {
			printf("Exit the macro first [qmacro]\n");
			continue;
		}
		
		// Exit macro
		if (strcmp(buf_msg, "qmacro") == 0) {
			if (pid != -1) { 
				// Kill child process
				if (!kill(pid, SIGINT)) {
					pid = -1;
					perror("Close Recv Process");
				}
			}
			continue;
		}
		
		// Active macro
		if (strcmp(buf_msg, "macro") == 0) {
			// Fork
			if ((pid = fork()) < 0) 
				perror("fork fail");
				
			// Child Process
			if (pid == 0) {
				while(1) {
					if (sendto(serverSocket, buf_msg, BUFSIZE, 0, (struct sockaddr *)&d_addr, sizeof(d_addr)) < 0) {
						perror("send fail");
						exit(1);
					}
					sleep(5);
				}
			}
			continue;
		}

		// normal msg
		if (sendto(serverSocket, buf_msg, BUFSIZE, 0, (struct sockaddr *)&d_addr, sizeof(d_addr)) < 0) {
			perror("send fail");
			return 0;
		}
	}

	return 0;
}
