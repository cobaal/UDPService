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
	char *manMsg = "How to use : tudp [<option 1>] .. [<option 2>] .. [<IP Address>] ... [<Port>]...\n\nThe options are described below.\n  -u,\t\tTransmits user-entered text.\n  -i [<msec>],\tTransmits text at a specific interval time.\n  -c [<msec>],\tTransmits Center packet at a specific interval time.\n  -t [<msec>],\tTransmits Train Packet at a specific interval time.\n\nHow to terminate : exit\n\n";

	int option = -1;

	// Check argv
	if (argc != 4 && argc != 5) {
		printf("%s", manMsg);
		return 0;
	}

	// Check Option
	if (strcmp(argv[1], "-u") == 0) option = 1;
	if (strcmp(argv[1], "-i") == 0) option = 2;
	if (strcmp(argv[1], "-c") == 0) option = 3;
	if (strcmp(argv[1], "-t") == 0) option = 4;

	// Check option
	if (!((argc == 4 && option == 1) || (argc == 5 && (option == 2 || option == 3 || option == 4)))) {
		printf("%s", manMsg);
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
	d_addr.sin_port = htons(atoi(argv[argc - 1]));		// Port #
	d_addr.sin_addr.s_addr = inet_addr(argv[argc - 2]);	// IPv4 Address		

	
	// Transmits user-entered text
	if (option == 1) {
		while (1) {
			// User input
			printf("%s:%s] ", argv[argc - 2], argv[argc - 1]);
			scanf("%s", buf_msg);

			// Exit process
			if (strcmp(buf_msg, "exit") == 0) 
				return 0;

			// Transmit msg
			if (sendto(serverSocket, buf_msg, BUFSIZE, 0, (struct sockaddr *)&d_addr, sizeof(d_addr)) < 0) {
				perror("send fail");
				return 0;
			}
		}

	// Transmits text packets at a specific interval time.
	} else if (option == 2) {
		int pid = -1;
		int msec = atoi(argv[2]);
		
		// User input
		printf("%s:%s] ", argv[argc - 2], argv[argc - 1]);
		scanf("%s", buf_msg);

		// Fork
		if ((pid = fork()) < 0) 
			perror("fork fail");

		// Child Process
		if (pid == 0) {				
			while (1) {
				if (sendto(serverSocket, buf_msg, BUFSIZE, 0, (struct sockaddr *)&d_addr, sizeof(d_addr)) < 0) {
					perror("send fail");
					exit(1);
				}
				usleep(msec * 1000);
			}

		// Parent Process
		} else {
			while (1) {
				// User input			
				printf("To Exit, input [exit] : ");
				scanf("%s", buf_msg);

				// Exit interval
				if (strcmp(buf_msg, "exit") == 0) {
					// Kill child process
					if (!kill(pid, SIGINT)) {
						pid = -1;
						perror("Close Process");
					}
					return 0;
				}
			}
		}

	// option 3~4
	} else {
		int pid = -1;
		int msec = atoi(argv[2]);

		// Fork
		if ((pid = fork()) < 0) 
			perror("fork fail");

		char buf_macro[128];
		int macro_size = sizeof(buf_macro);
		buf_macro[0] = 0x2f;
		buf_macro[1] = 0;

		if (option == 3) buf_macro[2] = 0x21;
		if (option == 4) buf_macro[2] = 0x42;

		// Child Process
		if (pid == 0) {				
			while (1) {
				if (sendto(serverSocket, buf_macro, macro_size, 0, (struct sockaddr *)&d_addr, sizeof(d_addr)) < 0) {
					perror("send fail");
					exit(1);
				}
				buf_macro[1] = (buf_macro[1] + 1) % 128; //0~127
				usleep(msec * 1000);
			}

		// Parent Process
		} else {
			while (1) {
				// User input			
				printf("To Exit, input [exit] : ");
				scanf("%s", buf_msg);

				// Exit interval
				if (strcmp(buf_msg, "exit") == 0) {
					// Kill child process
					if (!kill(pid, SIGINT)) {
						pid = -1;
						perror("Close Process");
					}
					return 0;
				}
			}
		}
	}

	return 0;
}
