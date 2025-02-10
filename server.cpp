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
#define BACKLOG 5 

void handleClient(int newsockfd, int clientNum, struct sockaddr_in clientAddr)
{
    char buffer[BUFFER_SIZE];
    int numOfBytes;

    while(1)
    {
        memset(buffer, 0, BUFFER_SIZE); 
        numOfBytes = read(newsockfd, buffer, BUFFER_SIZE); 
        if(numOfBytes == -1)
        {
            perror("ERROR - Reading data from client failed!");
            exit(EXIT_FAILURE);
        }
        if(strncmp("Quit.", buffer, 5) == 0)
        {
            std::cout << "Disconnected from " << inet_ntoa(clientAddr.sin_addr) << ":" << ntohs(clientAddr.sin_port) << std::endl;
            break;
        }
        std::cout << "\nClient " << clientNum << ": " << buffer << std::endl;

        memset(buffer, 0, BUFFER_SIZE); 
        std::cout << "\t\t\t\tServer: ";
        std::cin.getline(buffer, BUFFER_SIZE);  
        numOfBytes = write(newsockfd, buffer, BUFFER_SIZE); 
        if(numOfBytes == -1)
        {
            perror("ERROR - Writing data to client failed!");
            exit(EXIT_FAILURE);
        }
    }
    close(newsockfd);  
}

int main()
{
    int sockfd, newsockfd, clientNum;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t len = sizeof(clientAddr);
    pid_t child_process_id;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd == -1)
    {
        perror("ERROR - Creating the server socket failed!");
        exit(EXIT_FAILURE);
    }
    std::cout << "Server socket created successfully." << std::endl;

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT_NUM);

    if(bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
    {
        perror("ERROR - Binding the socket failed!");
        exit(EXIT_FAILURE);
    }
    if(listen(sockfd, BACKLOG) == -1)
    {
        perror("ERROR - Listening for connections failed!");
        exit(EXIT_FAILURE);
    }

    std::cout << "Server listening for connections..." << std::endl;
    clientNum = 0;
    while(1)
    {
        newsockfd = accept(sockfd, (struct sockaddr *)&clientAddr, &len);
        if(newsockfd == -1)
        {
            perror("ERROR - Accepting client connection failed!");
            exit(EXIT_FAILURE);
        }
        clientNum++;
        std::cout << "Client " << clientNum << " connected from " << inet_ntoa(clientAddr.sin_addr) << ":" << ntohs(clientAddr.sin_port) << std::endl;

        child_process_id = fork();
        if(child_process_id == 0)
        {
            close(sockfd);  
            handleClient(newsockfd, clientNum, clientAddr);  
            exit(0);  
        } 
        else if (child_process_id < 0)
        {
            perror("ERROR - Fork failed!");
            exit(EXIT_FAILURE);
        }
    }
    close(sockfd); 
    return 0;
}