# UnixDomainSockets

- Socket is one of the Inter Process Communication Mechanism.
- Basically here we create Server and Client. i.e, one server to one client communication,
  or one server to many client communication(i.e, Multiplexing)

- There are two types of Sockets:
  a. Unix Domain Sockets.
  b. Network Domain Sockets.

- Implementation of the Socket Involves below mentioned Steps:
- Server Side Implementation:
  1. socket()  - To create a master file descriptor(master_fd) which listens to the new Connection requests from Clients.
  2. bind()    - Bind the master_fd to the specific Port or Path Name (i.e, in general that Application).
  3. listen()  - Once the bind() is successfull, listen the new connection requests over that binded path or port 
                 (so listen is a Blocking    Call).
  4. accept()  - Using accept connection is established between the client and server - returns the new file desciptor for the 
    	           communication between the server and client.
  5. close()   - System call to close the opened file File Descriptor.

- Client side Implementation:
  1. socket()  - To create the communication socket.
  2. connect() - To initate the connection with the server.

- And for Communiction between server and client, read/write can be used.

- For Multiplexing - select() and fd_set data structure are used in the Multiplex_server.cpp
