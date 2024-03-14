#include <cstring> 
#include <iostream> 
#include <netinet/in.h> 
#include <sys/socket.h> 
#include <unistd.h> 
#include <arpa/inet.h> 
  
int main() 
{
    // creating socket 
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0); 
  
    // specifying address 
    // sockaddr_in serverAddress; 
    // serverAddress.sin_family = AF_INET; 
    // serverAddress.sin_port = htons(8081); // Server's port number in network byte order
    // inet_pton(AF_INET, "127.0.0.1", &serverAddress.sin_addr); // Server's IP address
    // serverAddress.sin_addr.s_addr = INADDR_ANY; 
    // inet_pton(AF_INET, "127.0.0.1", &serverAddress.sin_addr); 
    // inet_pton(AF_INET, "192.168.1.5", &serverAddress.sin_addr); // -> This is when the server is not on the same machine.

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8081);
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);
  
    // sending connection request 
    // connect(clientSocket, (struct sockaddr*)&serverAddr, 
    //         sizeof(serverAddress)); 

    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
            perror("Connection failed bruuuh");
            exit(EXIT_FAILURE);
     }     

    // sending data 
    const char* message = "Hello, server!"; 
    send(clientSocket, message, strlen(message), 0); 
  
    // closing socket 
    close(clientSocket); 
  
    return 0; 
}