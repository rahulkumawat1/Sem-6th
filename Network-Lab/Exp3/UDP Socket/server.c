// Server side implementation of UDP client-server model
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

void error(char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    int sockfd, port_val;
    char buffer[255];
    char *msg = "Hello Im Server";
    struct sockaddr_in server_addr, client_addr;

    if (argc < 2)
        error("Specify port number");

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (sockfd < 0)
        error("Socket creation failed");

    bzero((char *)&server_addr, sizeof(server_addr));
    bzero((char *)&client_addr, sizeof(client_addr));

    port_val = atoi(argv[1]);

    server_addr.sin_family = AF_INET; // IPv4
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port_val);

    //Binding...
    if (bind(sockfd, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
        error("Binding failed");

    int len, n;

    len = sizeof(client_addr);

    bzero(buffer, 255);

    n = recvfrom(sockfd, (char *)buffer, 255, MSG_WAITALL, (struct sockaddr *)&client_addr, &len);

    printf("Client : %s\n", buffer);
    sendto(sockfd, (const char *)msg, strlen(msg), MSG_CONFIRM, (const struct sockaddr *)&client_addr, len);
    printf("message sent.\n");

    return 0;
}
