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
#define BACKLOG 5 

int main()
{
	int sockfd, newsockfd, clientNum, numofBytes;    
	struct sockaddr_in serverAddr, clientAddr;  

	socklen_t len; 

	char buffer[BUFFER_SIZE]; 
	pid_t child_process_id;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd == -1)
    {
		perror("ERROR - Creating the server socket is failed");  
		exit(EXIT_FAILURE); 
	}
	printf("\nServer socket is created successfully.\n\n"
    "********************************* WELCOME TO MY CHAT ROOM *********************************\n"
    "- I'm glad to have you in my chat room and I'm looking forward to respond to your messages.\n"
    "- Please just type 'Bye.' when you are leaving.\n\n");      

	memset(&serverAddr, 0, sizeof(serverAddr));   
	serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY; 
	serverAddr.sin_port = htons(portNum); 

	if(bind(sockfd, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) == -1)
    {
		perror("ERROR - Binding the socket is failed"); 
		exit(EXIT_FAILURE); 
	}

	if(listen(sockfd, BACKLOG) == -1)
    {
		perror("ERROR - Listening for connections is failed"); 
		exit(EXIT_FAILURE); 
	}
    printf("Listening for connections...\n"); 

    len = sizeof(clientAddr); 
    clientNum = 0;  
	while(1)
    {
		newsockfd = accept(sockfd, (struct sockaddr *) &clientAddr, &len); 
		if(newsockfd == -1)
        {
			exit(EXIT_FAILURE);
		}
        clientNum++; 
		printf("Connected to %s:%d\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
        child_process_id = fork();   

		if(child_process_id == 0) 
        {
			close(sockfd);

			while(1)
            {
				read(newsockfd, buffer, BUFFER_SIZE); 
                printf("\nClient %d: %s\n", clientNum, buffer);
				if(!strncmp("Bye.", buffer, 3))   
                {
					printf("Disconnected from %s:%d\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
					break;
				}
                
                memset(buffer, 0, BUFFER_SIZE); 
                 
                printf("\t\t\t\t Server: ");    
                fgets(buffer, BUFFER_SIZE, stdin); 
                numofBytes = write(newsockfd, buffer, BUFFER_SIZE); 

                if(numofBytes == -1)
                {
                    perror("ERROR - Writing data to the client is failed.");   
                    exit(EXIT_FAILURE); 
                }
            }
        }
	}

	close(newsockfd); 

	return 0;
}