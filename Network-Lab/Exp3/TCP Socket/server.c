#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Please provide port number!");
        exit(1);
    }

    int sockfd, newsockfd, port_no;
    char buffer[255];

    struct sockaddr_in server_addr, client_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0); //TCP

    if (sockfd < 0)
    {
        error("Error opening socket");
    }

    // socket created...

    bzero((char *)&server_addr, sizeof(server_addr));
    port_no = atoi(argv[1]);

    server_addr.sin_family = AF_INET;         //IP Version
    server_addr.sin_addr.s_addr = INADDR_ANY; //IP address
    server_addr.sin_port = htons(port_no);    //Port no in network byte order ==> most sign byte first

    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
        error("Binding failed");

    //Binding is done...

    listen(sockfd, 5);
    socklen_t client_addr_len = sizeof(client_addr);

    newsockfd = accept(sockfd, (struct sockaddr *)&client_addr, &client_addr_len);

    if (newsockfd < 0)
        error("Error in Accept");

    //request accepted...

    int n;
    while (1)
    {
        bzero(buffer, 255);
        n = read(newsockfd, buffer, 255);
        if (n < 0)
            error("Error in Read");

        printf("Client: %s", buffer);
        bzero(buffer, 255);
        fgets(buffer, 255, stdin);

        n = write(newsockfd, buffer, strlen(buffer));
        if (n < 0)
            error("Error in Writing");

        int i = strncmp("Bye", buffer, 3);
        if (i == 0)
            break;
    }

    close(newsockfd);
    close(sockfd);

    return 0;
}