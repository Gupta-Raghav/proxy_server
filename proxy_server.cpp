#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

using namespace std;

sem_t semaphore; // Semaphore to limit concurrent connections

void* handleClient(void* arg) {
    int clientSocket = *((int*)arg);
    free(arg); // Free the allocated memory for the client socket

    // Receiving data
    char buffer[1024] = {0};
    recv(clientSocket, buffer, sizeof(buffer), 0);
    cout << "Message from client: " << buffer << endl;

    // Close client socket
    close(clientSocket);

    // Signal semaphore that thread is finished
    sem_post(&semaphore);
    
    return nullptr;
}

int main(int argc, char *argv[]) {
    int MAX_CLIENTS = 5;
    int PORT_NUMBER = 8080;

    // Initialize semaphore
    sem_init(&semaphore, 0, MAX_CLIENTS);

    // Creating a socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(serverSocket < 0) {
        perror("[Error]: Error in creating a socket. \n");
        exit(1);
    }

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT_NUMBER);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    if(bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        perror("Port is not available.");
        exit(1);
    }

    // Listening to the assigned socket
    if(listen(serverSocket, MAX_CLIENTS) < 0) {
        perror("Error while listening.");
        exit(1);
    }

    cout << "Server is listening on port " << PORT_NUMBER << endl;

    while(1) {
        sem_wait(&semaphore); // Wait on semaphore before accepting new connections
        
        int* clientSocket = (int*)malloc(sizeof(int));
        *clientSocket = accept(serverSocket, nullptr, nullptr);

        if(*clientSocket < 0) {
            perror("[ERROR]: Failed to accept the connection.");
            sem_post(&semaphore); // Release semaphore if accept fails
            free(clientSocket); // Free allocated memory
            continue;
        }

        cout << *clientSocket << ": Client Connected." << endl;

        // Create a thread to handle the new connection
        pthread_t thread_id;
        if(pthread_create(&thread_id, nullptr, handleClient, (void*)clientSocket) != 0) {
            perror("Failed to create thread");
            close(*clientSocket); // Close the client socket if thread creation fails
            sem_post(&semaphore); // Release semaphore
            free(clientSocket); // Free allocated memory
        } else {
            // Detach thread to free resources once finished
            pthread_detach(thread_id);
        }
    }
    
    // Closing the server socket
    close(serverSocket);

    // Destroy semaphore
    sem_destroy(&semaphore);

    return 0;
}
