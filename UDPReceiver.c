#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <signal.h>
#include <arpa/inet.h>
#include <time.h>
#include <sys/time.h>

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

		// for count
		double count_originPacket = 0.0;
		double count_fakePacket = 0.0;

		// for time
		struct timeval val;
		struct tm *t;

		printf("RECEIVER : Waiting Request. [(Q)uit]\n");
		while(1) {
			// Receive
			nbyte = recvfrom(serverSocket, buf_rcv, BUFSIZE, 0, (struct sockaddr *)&client_addr, &client_addr_size);
			if (nbyte < 0) {
				perror("recvfrom fail");
				exit(1);
			}

			// Get Current Time
			gettimeofday(&val, NULL);
			t = localtime(&val.tv_sec);

			printf("[%02d:%02d:%02d.%06ld] ", t->tm_hour, t->tm_min, t->tm_sec, val.tv_usec);

			// Output
			if (buf_rcv[0] == 0x2f) { // Receive Origin Packet
				printf("[%s] 0x%x 0x%x 0x%x\n", inet_ntoa(client_addr.sin_addr), buf_rcv[0], buf_rcv[1], buf_rcv[2]);
				count_originPacket++;

			} else if (buf_rcv[0] == 0x46) { // Receive Fake Packet
				printf("[%s] 0x%x 0x%x 0x%x\n", inet_ntoa(client_addr.sin_addr), buf_rcv[0], buf_rcv[1], buf_rcv[2]);
				count_fakePacket++;

			} else if (buf_rcv[0] == 0x45) { // Receive Stop Pacekt
				printf("\n############# Receive Stop Packet #############\n");
				printf("Origin Packet\t] %lf\n", count_originPacket);
				printf("Fake Packet\t] %lf\n", count_fakePacket);
				printf("###############################################\n");
				return 0;

			} else { // Receive User Message
				printf("[%s] %s\n", inet_ntoa(client_addr.sin_addr), buf_rcv);
			}
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
