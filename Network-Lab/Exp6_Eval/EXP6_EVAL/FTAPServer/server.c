#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#define SIZE 500

char s[100][100];

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

void split(char *str, char *delm)
{
    char *token = strtok(str, delm);

    int i = 0;
    bzero(s[0], 100);
    bzero(s[1], 100);

    while (i < 2)
    {
        strcpy(s[i++], token);
        printf("%s %d\n", token, strlen(token));
        token = strtok(NULL, "-");
    }
    s[1][strlen(s[1]) - 1] = '\0';
    printf("%s %d\n", s[1], strlen(s[1]));
}

int check_uname(char *uname)
{
    //fscanf(fptr, "%[^\n]", c)
    FILE *fp = fopen("../logincred.txt", "r");
    // printf("Hey1\n");
    char c[100];
    int i = 1;

    while (fgets(c, 100, fp) != NULL)
    {
        char *delm = ",";
        split(c, delm);
        // printf("%d: %c RK %s RK %s\n", i, c, uname, s[0]);
        if (strncmp(uname, s[0], strlen(uname)) == 0)
        {
            fclose(fp);
            return i;
        }
        i++;
        bzero(c, 100);
    }
    fclose(fp);
    return -1;
}

int check_pass(char *pass, int unum)
{
    int i = 1;
    FILE *fp = fopen("../logincred.txt", "r");
    char c[100];
    while (i < unum)
    {
        fgets(c, 100, fp);
        i++;
    }
    bzero(c, 100);
    fgets(c, 100, fp);
    char *delm = ",";
    split(c, delm);
    if (strncmp(pass, s[1], strlen(pass)) == 0)
    {
        return 1;
    }
    return -1;
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
    int auth = 0;
    int unum = -1;
    char uname[100];
    char pass[100];
    int filenum = 1;

    while (1)
    {
        bzero(buffer, 255);
        n = recv(newsockfd, buffer, 255, 0);
        if (n < 0)
            error("Error in Read");

        printf("Client: %s", buffer);

        if (auth == 0)
        {

            if (strncmp("START", buffer, 5) == 0)
            {
                bzero(buffer, 255);
                sprintf(buffer, "200 OK\n");
                send(newsockfd, buffer, strlen(buffer), 0);
            }
            else if (strncmp("USERN", buffer, 5) == 0)
            {
                char *delm = " ";
                split(buffer, delm);
                strcpy(uname, s[1]);

                unum = check_uname(uname);

                if (unum != -1)
                {
                    bzero(buffer, 255);
                    sprintf(buffer, "300\n");
                    send(newsockfd, buffer, strlen(buffer), 0);
                }
                else
                {
                    bzero(buffer, 255);
                    sprintf(buffer, "301\n");
                    send(newsockfd, buffer, strlen(buffer), 0);
                }
            }
            else if (strncmp("PASSWD", buffer, 6) == 0)
            {
                split(buffer, " ");
                strcpy(pass, s[1]);

                if (check_pass(pass, unum) == 1)
                {
                    bzero(buffer, 255);
                    sprintf(buffer, "305\n");
                    send(newsockfd, buffer, strlen(buffer), 0);
                    auth = 1;
                }
                else
                {
                    bzero(buffer, 255);
                    sprintf(buffer, "310\n");
                    send(newsockfd, buffer, strlen(buffer), 0);
                }
            }
            else
            {
                bzero(buffer, 255);
                sprintf(buffer, "Pls Authentic\n");
                send(newsockfd, buffer, strlen(buffer), 0);
            }
        }
        else
        {

            if (strncmp("CreateFile", buffer, 10) == 0)
            {
                char filename[9];
                sprintf(filename, "file%d.txt", filenum++);
                FILE *fp1 = fopen(filename, "w");
                fclose(fp1);

                bzero(buffer, 255);
                sprintf(buffer, "DONE\n");
                send(newsockfd, buffer, strlen(buffer), 0);
            }
            else if (strncmp("ListDir", buffer, 7) == 0)
            {
                char files[255] = {0};
                char src[9] = {0};

                for (int k = 1; k < filenum; k++)
                {
                    bzero(src, 5);
                    sprintf(src, "file%d.txt\n", k);
                    strcat(files, src);
                }

                send(newsockfd, files, strlen(files), 0);
            }
            else if (strncmp("StoreFile", buffer, 9) == 0)
            {
            }
            else if (strncmp("GetFile", buffer, 7) == 0)
            {
            }
            else if (strncmp("QUIT", buffer, 4) == 0)
            {
                break;
            }
            else
            {
                bzero(buffer, 255);
                sprintf(buffer, "505\n");
                send(newsockfd, buffer, strlen(buffer), 0);
            }
        }

        // if (strncmp("Bye", buffer, 3) == 0)
        //     break;
        // else if (strncmp("GivemeyourVideo", buffer, sizeof("GivemeyourVideo") - 1) == 0)
        // {
        //     bzero(buffer, 255);
        //     sprintf(buffer, "File Sending...\n");
        //     send(newsockfd, buffer, strlen(buffer), 0);

        //     recv(newsockfd, buffer, 1, 0); //ACK

        //     FILE *fp;
        //     char *filename = "file.dat";
        //     fp = fopen(filename, "r");

        //     if (fp == NULL)
        //     {
        //         printf("[-]Error in reading...");
        //         exit(1);
        //     }

        //     send_file(fp, newsockfd);

        //     bzero(buffer, 255);
        //     sprintf(buffer, "FILE_SENT");
        //     send(newsockfd, buffer, strlen(buffer), 0);

        //     fclose(fp);
        // }
        // else
        // {
        //     bzero(buffer, 255);
        //     sprintf(buffer, "Message recieved\n");

        //     n = send(newsockfd, buffer, strlen(buffer), 0);
        //     if (n < 0)
        //         error("Error in Writing");
        // }
    }

    close(newsockfd);
    close(sockfd);

    return 0;
}