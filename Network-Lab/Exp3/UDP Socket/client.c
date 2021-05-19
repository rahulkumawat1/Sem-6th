#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>

void error(char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    int sockfd, port_no;
    char buffer[255];
    char *msg = "Hello I'm client";
    struct sockaddr_in server_addr;
    struct hostent *server;

    if (argc < 3)
        error("Port is not specified");

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (sockfd < 0)
        error("Socket creation failed");

    bzero((char *)&server_addr, sizeof(server_addr));

    port_no = atoi(argv[2]);
    server = gethostbyname(argv[1]);

    if (server == NULL)
        error("Server problem");

    // Filling server information
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port_no);

    bcopy((char *)server->h_addr, (char *)&server_addr.sin_addr.s_addr, server->h_length);

    int n, len;

    sendto(sockfd, (const char *)msg, strlen(msg), MSG_CONFIRM, (const struct sockaddr *)&server_addr, sizeof(server_addr));
    printf("message sent.\n");

    bzero(buffer, 255);
    n = recvfrom(sockfd, (char *)buffer, 255, MSG_WAITALL, (struct sockaddr *)&server_addr, &len);
    printf("Server : %s\n", buffer);

    close(sockfd);
    return 0;
}
