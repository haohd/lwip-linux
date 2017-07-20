# lwip-linux
This is a version of lwip running on Ubuntu

1. Ubuntu Setup: Install libpcap-dev

   sudo apt-get install libpcap-dev
	
2. Compile
   Go to the folder ./Debug, then run:
   
   make clean; make 
	
3. Run tests: Must be run under root user 
3.1 Echo server test 
	Under the header file "./lwip-2.0.2/test/linux/lwip.h", set TEST_ID to ECHO_SERVER
		
	#define TEST_ID 	ECHO_SERVER
		
	Then, re-compile the source code, and run the lwip-linux app. Use puty to connect to server at port 6677. 


3.2 TCP client test 
	Under the header file "./lwip-2.0.2/test/linux/lwip.h", set TEST_ID to TCP_CLIENT
	
	#define TEST_ID 	TCP_CLIENT
	
	Then, re-compile the source code, and run the lwip-linux app. 
	
	
	Notes:
		- The remote server address is being hard code at "192.168.2.12", we can modify this address via macro TCP_REMOTE_SERVER_ADDR under ./lwip-2.0.2/test/linux/lwip.h.
		- Remote server port is 6677, it can be changed via macro TCP_REMOTE_SERVER_PORT. 
		

4. Other notes 
	- lwip-linux only supports 32 local server ports from 6677 to 6709. When we create a tcp server, please use the server port in this range.
	- For local client ports, the lwip-linux supports to allocate port in range from 49152 to 49184.  
	
	
