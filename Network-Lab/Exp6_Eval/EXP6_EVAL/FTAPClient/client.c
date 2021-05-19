#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#define SIZE 500

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

void write_file(int sockfd)
{
    int n;
    FILE *fp;
    char *filename = "recv.dat";
    char buffer[SIZE];

    char *ack;

    fp = fopen(filename, "w");
    while (1)
    {
        n = recv(sockfd, buffer, SIZE, 0);
        if (n <= 0)
            break;

        if (strncmp("FILE_SENT", buffer, 9) == 0)
        {
            printf("Server: File Sent.\n");
            break;
        }

        fprintf(fp, "%s", buffer);

        send(sockfd, ack, 1, 0); //ACK

        bzero(buffer, SIZE);
    }
    return;
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("Please provide port number!");
        exit(1);
    }

    int sockfd, port_no;
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
        send(sockfd, buffer, sizeof(buffer), 0);

        if (strncmp("QUIT", buffer, 4) == 0)
        {
            break;
        }

        bzero(buffer, 255);
        n = recv(sockfd, buffer, 255, 0);
        if (n < 0)
            error("Error in Reading");

        printf("Server: %s", buffer);

        // if (strncmp("START", buffer, 5) == 0){

        // }
        // else if (strncmp("USERN", buffer, 5) == 0){

        // }
        // else if (strncmp("PASSWD", buffer, 6) == 0){

        // }
        // else if (strncmp("CreateFile", buffer, 6) == 0){

        // }
        // else if(strcm)

        //     else if (strncmp("GivemeyourVideo", buffer, sizeof("GivemeyourVideo") - 1) == 0)
        //     {
        //         bzero(buffer, 255);
        //         n = recv(sockfd, buffer, 255, 0);
        //         if (n < 0)
        //             error("Error in Reading");

        //         printf("Server: %s", buffer);

        //         send(sockfd, buffer, 1, 0); //ACK

        //         write_file(sockfd);
        //     }
        // else
        // {
        //     bzero(buffer, 255);
        //     n = recv(sockfd, buffer, 255, 0);
        //     if (n < 0)
        //         error("Error in Reading");

        //     printf("Server: %s", buffer);
        // }
    }

    close(sockfd);
    return 0;
}