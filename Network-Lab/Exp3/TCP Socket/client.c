#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("Please provide port number!");
        exit(1);
    }

    int sockfd, newsockfd, port_no;
    char buffer[255];

    struct sockaddr_in server_addr;
    struct hostent *server; //a struct wich is used to store info about host such as IPv4 addr, hostname

    port_no = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0)
    {
        error("Error opening socket");
    }

    // socket created...
    server = gethostbyname(argv[1]);

    if (server == NULL)
    {
        error("No such host");
    }

    //got server..
    bzero((char *)&server_addr, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&server_addr.sin_addr.s_addr, server->h_length);
    server_addr.sin_port = htons(port_no);

    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
        error("connection failed");

    //Connection suceed...
    int n;

    while (1)
    {
        bzero(buffer, 255);
        fgets(buffer, 255, stdin);
        n = write(sockfd, buffer, sizeof(buffer));

        if (n < 0)
            error("Error in Writing");

        bzero(buffer, 255);
        n = read(sockfd, buffer, 255);
        if (n < 0)
            error("Error in Reading");

        printf("Server: %s", buffer);

        int i = strncmp("Bye", buffer, 3);
        if (i == 0)
            break;
    }

    close(sockfd);
    return 0;
}