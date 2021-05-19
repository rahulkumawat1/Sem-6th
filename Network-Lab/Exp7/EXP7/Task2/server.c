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

void send_new(int fd, char *msg)
{
    printf("%s", msg);
    int len = strlen(msg);
    if (send(fd, msg, len, 0) == -1)
    {
        printf("Error in send\n");
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
    char buffer[1];

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

    //connection(newsockfd);

    bzero(buffer, 1);
    int flag = 0;
    while (recv(newsockfd, buffer, 1, 0) > 0)
    {
        printf("%s", buffer);

        if (flag == 0 && strncmp(buffer, "\r", 1) == 0)
            flag = 1;
        else if (flag == 1 && strncmp(buffer, "\n", 1) == 0)
            flag = 2;
        else if (flag == 2 && strncmp(buffer, "\r", 1) == 0)
            flag = 3;
        else if (flag == 3 && strncmp(buffer, "\n", 1) == 0)
            break;
        else
            flag = 0;

        bzero(buffer, 1);
    }

    printf("Response:\n");

    send_new(newsockfd, "HTTP/1.1 200 OK\r\n");
    send_new(newsockfd, "Server: Server in c\r\n");
    send_new(newsockfd, "Content-Type: text/html\r\n");
    send_new(newsockfd, "\r\n");
    send_new(newsockfd, "<body><p>Welcome to Networks Lab!</p></body></html>");

    close(newsockfd);
    close(sockfd);

    return 0;
}