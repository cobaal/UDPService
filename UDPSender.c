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

	char msg[512];
	char dip[32];
	char dport[32];
	int BUFSIZE = 512;
	char buf_msg[BUFSIZ];

	while(1){
		printf("Input Destination IP and Port : ");
		scanf("%s %s", dip, dport);

		// Close Sender Process
		if (strcmp(dip, "exit") == 0 & strcmp(dport, "-1") == 0) break;

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
		d_addr.sin_port = htons(atoi(dport));		// Port #
		d_addr.sin_addr.s_addr = inet_addr(dip);	// IPv4 Address		

		while(1) {
			printf("%s:%s] ", dip, dport);
			scanf("%s", buf_msg);

			if (strcmp(buf_msg, "exit") == 0) break;

			if (sendto(serverSocket, buf_msg, BUFSIZE, 0, (struct sockaddr *)&d_addr, sizeof(d_addr)) < 0) {
				perror("send fail");
				break;
			}
		}
	}

	return 0;
}
