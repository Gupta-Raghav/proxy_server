#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

using namespace std;
int i=0;
// Thread function to connect to the server
void* connectToServer(void* arg) {
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080); // Server's port number
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr); // Server's IP address

    // Creating socket
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0) {
        perror("[-]Error in connection.");
        exit(1);
    }
    cout << "[+]Client Socket is created.\n";

    // Sending connection request
    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("[-]Error in connection.");
        exit(1);
    }
    cout << "[+]Connected to Server.\n";

    // Sending data
    string message = "Hello, server! " + std::to_string(i++);
    const char* c_message = message.c_str();
    send(clientSocket, c_message, strlen(c_message), 0);

    // Closing socket
    close(clientSocket);
    return nullptr;
}

int main() {
    int NUM_CLIENTS = 10; // Number of simultaneous client connections to attempt
    pthread_t threads[NUM_CLIENTS];

    // Create and launch threads to simulate multiple clients
    for (int i = 0; i < NUM_CLIENTS; ++i) {
        if (pthread_create(&threads[i], nullptr, connectToServer, nullptr) != 0) {
            perror("Failed to create thread");
        }
    }

    // Wait for all threads to finish
    for (int i = 0; i < NUM_CLIENTS; ++i) {
        pthread_join(threads[i], nullptr);
    }

    return 0;
}
