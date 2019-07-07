//
// Created by Caitlin on 8/06/2019.
//

#include <iostream>
#include <unistd.h>
#include "server.hh"
#include "database.hh"
#include <cstring>
#include <sstream>

using namespace std;
using namespace database;

/**
 * Create a socket
 * @return
 */
int createSocket() {
    int fd= socket(AF_INET, SOCK_STREAM, 0);
    if (fd== -1) {
        printf("Error creating socket");
        exit(1);
    }
    printf("Created socket \n");
    return fd;

}
/**
 * Bind a socket
 */
void bindSocket( int socket, sockaddr_in address) {
    if (bind(socket, (struct sockaddr*)&address, sizeof(address))<0) {
        printf("Error binding socket");
        exit(1);
    }
    printf("Bound socket \n");
}

/**
 * Listen to a socket
 * @param fd
 */
void listenSocket(int fd) {
    if(listen(fd, SOMAXCONN) < 0) {
        printf("Error listening for connections");
        exit(1);
    }
}

/**
 * Accept a connection
 * @param fd
 * @param client_addr
 * @param addrlen
 * @return
 */
int acceptConnect(int fd, struct sockaddr_in client_addr, socklen_t addrlen) {
    int client_fd= accept(fd, (struct sockaddr*)&client_addr, (socklen_t*)&addrlen);
    if(client_fd< 0) {
        printf("Error accepting connection");
        exit(1);
    }
    printf("Accepted a Client \n");
    return client_fd;
}

/**
 * Serve Client, send and receive messages
 * @param client_fd
 */
void serveClient(int client_fd) {
    char msg[] = "hello, world \n";
    ssize_t r = send(client_fd, msg, strlen(msg), 0);
    if (r < 0) {
        printf("Error sending message \n");
        close(client_fd);
        exit(1);
    }
    char buffer[100];
    DbTable* db;
    db = new DbTable;
    db->loadCSV("scifi.csv"); //Load in csv file into database
    while (strcasecmp(buffer,"BYE\n") !=0) { //While they haven't sent bye
        memset(buffer,0,100);
        ssize_t r = recv(client_fd, buffer, 100, 0);
        if(r <= 0) {
            printf("Error sending message \n");
            close(client_fd);
            exit(1);
        }
        string command, arg,output;
        istringstream in(buffer);
        ostringstream out;

        in>>command>>arg;

        if(strcasecmp(command.c_str(),"GET")==0) { //If they have sent bye
            if(arg.empty()) { //If there is no following argument then show all the database
                out<<db->showAll();
                output = out.str();
            } else {
                try { //Else try and get the row specified but if it is out of bounds or not a number then throw an error message
                    output = db->showRow((unsigned int)stoul(arg));
                } catch (...) {
                    output = "Error: Invalid Row \n";
                }
            }
        } else if (strcasecmp(buffer,"BYE\n") !=0) { //Check that they have sent bye or get
            output = "Error: Should be GET or BYE \n";
        }
        send(client_fd,output.c_str(),output.length(),0); //Send
    }
    //Close the connection and delete the databse
    send(client_fd,"Disconnect",strlen("Disconnect"),0);
    close(client_fd);
    printf("Closed Connection \n");
    delete db;
}


int main() {
    struct sockaddr_in addr = {};
    struct sockaddr_in clientaddr = {};
    int addrLen = sizeof(clientaddr);
    int fd = createSocket();
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(12345);
    bindSocket(fd, addr);
    while (true) {
        listenSocket(fd);
        int client_fd = acceptConnect(fd, clientaddr, addrLen);
        pid_t p = fork(); //Fork
        //If fork is a failure then just serve the client
        if(p < 0) {
            serveClient(client_fd);
            wait(nullptr);
        } else if (p ==0) { //If it is the child then serve the client and once it is finished then it exits
            serveClient(client_fd);
            exit(0);
            break;
        } else { //Else close the current client
            close(client_fd);
        }
    }
    return 0;
}

