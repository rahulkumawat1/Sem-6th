#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#define SIZE 500

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

void send_file(FILE *fp, int sockfd)
{
    int n;
    char data[SIZE] = {0};
    char *ack;
    long long int k = 0;

    while (fgets(data, SIZE, fp) != NULL)
    {
        if (send(sockfd, data, strlen(data), 0) == -1)
        {
            perror("[-]Error in sending file.");
            exit(1);
        }
        recv(sockfd, ack, 1, 0); //ACK
        k += 50;
        bzero(data, SIZE);
    }
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
        n = recv(newsockfd, buffer, 255, 0);
        if (n < 0)
            error("Error in Read");

        printf("Client: %s", buffer);

        if (strncmp("Bye", buffer, 3) == 0)
            break;
        else if (strncmp("GivemeyourVideo", buffer, sizeof("GivemeyourVideo") - 1) == 0)
        {
            bzero(buffer, 255);
            sprintf(buffer, "File Sending...\n");
            send(newsockfd, buffer, strlen(buffer), 0);

            recv(newsockfd, buffer, 1, 0); //ACK

            FILE *fp;
            char *filename = "file.dat";
            fp = fopen(filename, "r");

            if (fp == NULL)
            {
                printf("[-]Error in reading...");
                exit(1);
            }

            send_file(fp, newsockfd);

            bzero(buffer, 255);
            sprintf(buffer, "FILE_SENT");
            send(newsockfd, buffer, strlen(buffer), 0);

            fclose(fp);
        }
        else
        {
            bzero(buffer, 255);
            sprintf(buffer, "Message recieved\n");

            n = send(newsockfd, buffer, strlen(buffer), 0);
            if (n < 0)
                error("Error in Writing");
        }
    }

    close(newsockfd);
    close(sockfd);

    return 0;
}