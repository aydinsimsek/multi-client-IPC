#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024  
#define PORT_NUM 5001 

int main()
{
    int sockfd, numOfBytes;
    struct sockaddr_in serverAddr;
    char buffer[BUFFER_SIZE];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd == -1)
    {
        perror("ERROR - Creating the client socket failed!");
        exit(EXIT_FAILURE);
    }
    std::cout << "Client socket created successfully." << std::endl;

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY; // Using localhost for testing
    serverAddr.sin_port = htons(PORT_NUM);

    if(connect(sockfd, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) == -1)
    {
        perror("ERROR - Connecting to the server failed!");
        exit(EXIT_FAILURE);
    }
    std::cout << "Connected to the server." << std::endl;

    while(1)
    {
        memset(buffer, 0, BUFFER_SIZE); 
        std::cout << "\t\t\t\tMe: "; 
        std::cin.getline(buffer, BUFFER_SIZE);  

        numOfBytes = write(sockfd, buffer, BUFFER_SIZE);  
        if(numOfBytes == -1)
        {
            perror("ERROR - Writing data to server failed!");
            exit(EXIT_FAILURE);
        }

        if(strncmp("Quit.", buffer, 5) == 0)
        {  
            std::cout << "Disconnected from the server." << std::endl;
            break;
        }

        memset(buffer, 0, BUFFER_SIZE);
        numOfBytes = read(sockfd, buffer, BUFFER_SIZE);  
        if(numOfBytes == -1)
        {
            perror("ERROR - Reading data from server failed!");
            exit(EXIT_FAILURE);
        }
        std::cout << "Server: " << buffer << std::endl;  
    }
    close(sockfd);  
    return 0;
}
