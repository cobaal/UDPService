CC=gcc

all: udpreceiver udpsender

udpreceiver: UDPReceiver.c
	$(CC) -o $@ $^

udpsender: UDPSender.c
	$(CC) -o $@ $^

clean:
	rm udpreceiver udpsender
