#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <signal.h>
#include <arpa/inet.h>

void *socketReceive();
void *socketSend();

int main(int argc, char *argv[]) {

	// ARGV
	if (argc != 3) {
		printf("PLEASE INPUT IP & PORT#\n");
		return 0;
	}

	// Make UDP Socket Discriptor
	int serverSocket;
	printf("SERVER : Creating Server UDP Socket...\n");
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

	printf("SERVER : Binding...\n");
	if (bind(serverSocket, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) < 0) {
		perror("Bind Fail");
		exit(1);
	} else {
		printf("SERVER : Success Binding!\n");
	}

	// Fork
	int pid;
	
	if ((pid = fork()) < 0) {
		perror("fork fail");
		exit(1);
	}

	// Child Process
	if (pid == 0) {
		printf("Recv Pid : %d\n", getpid());

		// recv
		struct sockaddr_in client_addr;
		int client_addr_size;
		int nbyte;
		int BUFSIZE = 512;
		char buf_rcv[BUFSIZ];

		printf("SERVER : Waiting Request.\n");
		while(1) {
			nbyte = recvfrom(serverSocket, buf_rcv, BUFSIZE, 0, (struct sockaddr *)&client_addr, &client_addr_size);
			if (nbyte < 0) {
				perror("recvfrom fail");
				exit(1);
			}
	
			//printf("Client IP : %s\n", client_addr.sa_data);
			printf("Cleint Msg : %s\n", buf_rcv);
		}

	// Parent Process
	} else {
		char msg[512];
		char dip[32];
		char dport[32];
		int BUFSIZE = 512;
		char buf_msg[BUFSIZ];

		sleep(1);

		while(1){
			printf("Destination IP and Port : ");
			scanf("%s %s", dip, dport);

			// close process
			if (strcmp(dip, "exit") == 0 & strcmp(dport, "-1") == 0) {
				if (!kill(pid, SIGINT)) {
					perror("Close Recv Process");
				}
				return 0;
			}		

			// make destination addr
			struct sockaddr_in d_addr;
			memset(&d_addr, 0, sizeof(struct sockaddr));
			d_addr.sin_family = AF_INET;			// IPv4 Internet Protocol
			d_addr.sin_port = htons(atoi(dport));		// Port #
			d_addr.sin_addr.s_addr = inet_addr(dip);	// IPv4 Address		

			while(1) {
				printf("MSG : ");
				scanf("%s", buf_msg);

				if (strcmp(buf_msg, "exit") == 0) break;

				if (sendto(serverSocket, buf_msg, BUFSIZE, 0, (struct sockaddr *)&d_addr, sizeof(d_addr)) < 0) {
					perror("send fail");
					break;
				}
			}
		}
	}

	return 0;
}
