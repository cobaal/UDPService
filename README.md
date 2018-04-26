# UDPService

Copyright (c) 2018, Hyung-Seok Park

## Installing

	git clone https://github.com/cobaal/UDPService

## How to use

 - Compile
 
		root:~$ cd UDPService
		root:~/UDPService$ make 

 - Receiver Execution (Text)
 
		root:~/UDPService$ ./rudp -t [<IP Address>] [<Port>] 

 - Receiver Execution (Hex)
 
		root:~/UDPService$ ./rudp -h [<IP Address>] [<Port>] 

 - Receiver Termination

		Q

 - Sender Execution (user Text)

		root:~/UDPService$ ./tudp -u [<IP Address>] [<Port>]

 - Periodically Send Messages

		root:~/UDPService$ ./tudp -i [<msec>] [<IP Address>] [<Port>]

 - Sender Termination

		xxx.xxx.xxx.xxx:xxxx] exit
		

