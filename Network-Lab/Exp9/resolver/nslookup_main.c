#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#define PORT     8081
#define MAXLINE 1024
int ends_with(char *a,char *b)
{
    for(int i=0;i<strlen(a);i++)
    {
        if(a[i]==b[0])
        {
            for(int j=0;j<strlen(b);j++)
            {
                if(a[i+j]!=b[j])
                {
                    return 0;
                }   
            }
            return 1;
        }
    }
    return 0;    
}
int starts_with(char *a,char *b)
{
    if(a[0]==b[0])
    {
        for(int i=1;i<strlen(b);i++)
        {
            if(a[i]!=b[i])
                return 0;
        }
        return 1;
    }
    return 0;
}
void substring(char s[], char sub[], int p, int l) {
   int c = 0;
   
   while (c < l) {
      sub[c] = s[p+c-1];
      c++;
   }
   sub[c] = '\0';
}
int check_cache(char site_name[50],char type[50],char result[1000])
{
    system("ls -t cache/ > cachelist.txt");
    FILE* cachelist = fopen("cachelist.txt","r");

    char cache_file[50];
    while (fscanf(cachelist,"%s",cache_file)!=EOF)
    {
        if(starts_with(cache_file,site_name) && cache_file[strlen(site_name)]=='-')
        {
            // printf("%s\n",cache_file);
            int i=strlen(site_name);
            char type_cache[50];
            substring(cache_file,type_cache,i+2,strlen(cache_file)-1);
            if(strcmp(type,type_cache)==0)
            {
                printf("Found Cache\n");
                
                char filename[50]="cache/";
                strcat(filename,site_name);
                strcat(filename,"-");
                strcat(filename,type);
                
                FILE *cache_data=fopen(filename,"r");
                char line[50],data[1000];
                while (fgets(line, sizeof(line), cache_data) != NULL)
                {
                    // printf("%s",line);
                    strcat(data,line);
                }

                strcpy(result,data);
                return 1;
            }   
        }
    }

    FILE *cachelist_del=fopen("cachelist.txt","r");
    int max=1;
    char cache_file_del[50];
    while (fscanf(cachelist_del,"%s",cache_file_del)!=EOF)
    {
        max++;
        if(max==5)
        {
            char command[50];
            strcpy(command,"rm cache/");
            strcat(command,cache_file_del);
            printf("Command %s\n",command);
            system(command);
        }
    }
    return 0;
}

void goto_nslookup2(char domain[50],char *ip)
{
    //Command Execution
    char command[100]={0};
    strcat(command,"nslookup -type=ns ");
    strcat(command,domain);
    strcat(command," ");
    strcat(command,ip);
    strcat(command," > terminal_output/terminal2.txt");
    // printf("\n\nCommand : %s\n",command);
    system(command);
    FILE *terminal=fopen("terminal_output/terminal2.txt","r");


    // printf("Looking up nslookup 2 ... \n");
    
    char line[50];
    int flag=0;
    while (fscanf(terminal,"%s",line)!=EOF)
    {
        // printf("Line -> %s\n",line);
        if(starts_with(line,"="))
        {
            flag=1;
            continue;
        }
        if (flag)
        {
            strcpy(ip,line);
            break;
        }   
    }
    printf("Name server : %s\n",ip);


}
char* goto_nslookup(char site_name[100],char type[10],char result[1000])
{
    //Configure root server text file
    // printf("Looking into nslookup ... \n");
    // printf("Name of site : %s\n",site_name);
    // printf("\n\nCommand : nslookup -type=ns . > terminal_output/terminal.txt");
    system("nslookup -type=ns . > terminal_output/terminal.txt");
    
    FILE *terminal = fopen("terminal_output/terminal.txt","r");
    char rootdnsserver[100];
    char line[50];

    while (fscanf(terminal,"%s",line)!=EOF)
    {
        if(ends_with(line,".root-servers.net."))
        {
            // printf("%s\n",line);
            strcpy(rootdnsserver,line);
            break;
        }
    }
    
    printf("\nName of site : %s\n",site_name);
    printf("Root Name Server : %s\n",rootdnsserver);
    for(int i=strlen(site_name)-1;i>=0;i--)
    {
        int j=0;
        if(site_name[i]=='.')
        {
            char dest[50];
            substring(site_name,dest,i+2,strlen(site_name)-1);
            printf("%s\n",dest);   
            goto_nslookup2(dest,rootdnsserver);
            // break;
        }
    }
    // printf("Name Server : %s\n",rootdnsserver);
    fclose(terminal);


    char command[100]={0};
    strcat(command,"nslookup -type=ns ");
    strcat(command,site_name);
    strcat(command," ");
    strcat(command,rootdnsserver);
    strcat(command," > terminal_output/terminal3.txt");
    // printf("\n\nCommand : %s\n",command);
    system(command);
    FILE *terminal3=fopen("terminal_output/terminal3.txt","r");
    char data[1000]={0};
    int flag=0;
    while (fscanf(terminal3,"%s",line)!=EOF)
    {
        // printf("Line -> %s\n",line);
        if(starts_with(line,"="))
        {
            flag=1;
            continue;
        }
        if (flag)
        {
            strcpy(rootdnsserver,line);
            break;
        }   
    }

    printf("Name Server : %s\n",rootdnsserver);
    fclose(terminal3);


    // Type : A
    char command_a[100]={0};
    strcat(command_a,"nslookup -type=");
    strcat(command_a,type);
    strcat(command_a," ");
    if(strcmp(type,"cname")==0)
        strcat(command_a,"www.");
    strcat(command_a,site_name);
    strcat(command_a," ");
    strcat(command_a,rootdnsserver);
    strcat(command_a," > terminal_output/terminal3.txt");
    // printf("\n\nCommand : %s\n",command_a);
    system(command_a);
    
    FILE *terminal4=fopen("terminal_output/terminal3.txt","r");
    while (fgets(line, sizeof(line), terminal4) != NULL)
    {
        // printf("%s",line);
        strcat(data,line);
    }

    strcpy(result,data);
    
}
void handle_query(char site_name[100],char type[10],char result[1000])
{
    
    printf("\n");

    if(!check_cache(site_name,type,result))
    {
        goto_nslookup(site_name,type,result);

        char filename[50]="cache/";
        strcat(filename,site_name);
        strcat(filename,"-");
        strcat(filename,type);
        FILE *cache_store=fopen(filename,"a");

        fprintf(cache_store,"%s",result);
        fclose(cache_store);
    }
    // printf("\nTYPE : %s\nOUTPUT  :\n%s\n",type,result);
    // parsing result
    if(strcmp(type,"a")==0)
    {
        char *token=strtok(result," \t\n");
        while(token!=NULL)
        {
            // printf("%s\n",token);
            if(strcmp(site_name,token)==0)
            {
                token = strtok(NULL, " \t\n");
                token = strtok(NULL, " \t\n");
                strcpy(result,token);
                // printf(":%s:\n",result);
                break;
            }   
            token = strtok(NULL, " \t\n");
        }
    }
    else if(strcmp(type,"aaaa")==0)
    {
        // printf("yes\n");
        char *token=strtok(result," \t\n");
        int flag=0;
        while(token!=NULL)
        {
            printf("%s\n",token);
            if(strcmp("No",token)==0)
            {
                flag++;
                // printf("%s\n",token);
            }
            if(strcmp("can't",token)==0)
            {
                flag++;
                // printf("%s\n",token);
            }
            if(flag>0)
            {
                strcpy(result,"NO");
                break;
            }
            if(strcmp(site_name,token)==0)
            {
                token = strtok(NULL, " \t\n");
                token = strtok(NULL, " \t\n");
                strcpy(result,token);
                printf(":%s:\n",result);
                break;
            }
            token = strtok(NULL, " \t\n");
        }
    }
    else if(strcmp(type,"cname")==0)
    {
        char *token=strtok(result," \t\n");
        int flag=0;
        while(token!=NULL)
        {
            // printf("%s\n",token);
            if(strcmp("No",token)==0)
            {
                flag++;
                // printf("%s\n",token);
            }
            if(strcmp("can't",token)==0)
            {
                flag++;
                // printf("%s\n",token);
            }
            if(flag>0)
            {
                strcpy(result,"NO");
                break;
            }
            if(strcmp("=",token)==0)
            {
                token = strtok(NULL, " \t\n");
                strcpy(result,token);
                printf(":%s:\n",result);
                break;
            }
            token = strtok(NULL, " \t\n");
        }
    }

}
// Driver code
void main() 
{
    char site_name[100]="reddit.com";
    char type[10]="cname"; 
    char result[1000] ;
    handle_query(site_name,type,result);
    printf("%s\n",result);
}