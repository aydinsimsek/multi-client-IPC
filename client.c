#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024  
#define portNum 5001 

int main()
{
	int sockfd, numofBytes; 
	struct sockaddr_in serverAddr;
	char buffer[BUFFER_SIZE]; 

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd == -1)
    {
		perror("ERROR - Creating the client socket is failed");  
		exit(EXIT_FAILURE); 
	}
	printf("Client socket is created successfully.\n");  

	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY; 
	serverAddr.sin_port = htons(portNum);

	if(connect(sockfd, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) == -1)
    {
		perror("ERROR - Connecting to the server is failed"); 
		exit(EXIT_FAILURE); 
	}
	printf("\nConnected to server succesfully.\n");

	while(1)
    {
        memset(buffer, 0, BUFFER_SIZE); 
		printf("\n\t\t\t\t Me: ");   
		fgets(buffer, BUFFER_SIZE, stdin); 
		numofBytes = write(sockfd, buffer, BUFFER_SIZE); 

        if(numofBytes == -1)
        {
            perror("ERROR - Writing data to the server is failed.");   
            exit(EXIT_FAILURE); 
        }

		if(!strncmp("Bye.", buffer, 3))   
        {
			printf("Disconnected from the server.\n");
			break; 
		}

        memset(buffer, 0, BUFFER_SIZE);
        numofBytes = read(sockfd, buffer, BUFFER_SIZE); 

        if(numofBytes == -1)
        {
            perror("ERROR - Reading data from the server is failed.");   
            exit(EXIT_FAILURE); 
        }
		
        printf("Server: %s\n", buffer); 
	}

    close(sockfd); 
	return 0;
}