CC=gcc

all: rudp tudp

rudp: UDPReceiver.c
	$(CC) -o $@ $^

tudp: UDPSender.c
	$(CC) -o $@ $^

clean:
	rm rudp tudp
