//
// Created by Caitlin on 8/06/2019.
//

#ifndef UNTITLED_SERVER_HH
#define UNTITLED_SERVER_HH
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <arpa/inet.h>

//Create a socket
int createSocket();
//Bind a socket
void bindSocket(int,sockaddr_in);
//Listen for client
void listenSocket(int);
//Accept a client
int acceptConnect(int, struct sockaddr_in, socklen_t);
void serveClient(int);


#endif //UNTITLED_SERVER_HH
