#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h> 

using namespace std;

int main(int argc, char * argv[]){ //this is where I will make the call to the application.


    //creating a socket. 
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0); //this will create my scoket. we can say everytime a client tries to connect we will have to create a new socket.
    

    if(serverSocket < 0){
        perror("[Error]: Error in creating a socket. \n");
        exit(1);
    }

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080); //I should be defining the port somewhere else to better protect it. 
    serverAddress.sin_addr.s_addr =INADDR_ANY;

    bind(serverSocket, (struct sockaddr*)&serverAddress, 
         sizeof(serverAddress)); 
  
    // listening to the assigned socket 
    listen(serverSocket, 5); 
  
    // accepting connection request 
    int clientSocket 
        = accept(serverSocket, nullptr, nullptr); 
  
    // recieving data 
    char buffer[1024] = { 0 }; 
    recv(clientSocket, buffer, sizeof(buffer), 0); 
    cout << "Message from client: " << buffer 
              << endl; 
  
    // closing the socket. 
    close(serverSocket); 
  
    return 0; 

}

