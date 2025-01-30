#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void capitalize(char *str)
{
   int word=0;
   for(int i=0;str[i]!='\0';i++)
   {
      if(!word && str[i] >= 'a' && str[i] <='z')
      {
         str[i]=str[i]-32;
         word=1;
      }
      else if(str[i]==' ' || str[i]=='\t')
         word=0;
      else
         word=1;
   }
}
void main()
{
   int port=8800;
   struct sockaddr_in server_addr;
   bzero((char*)&server_addr,sizeof(server_addr));

   server_addr.sin_family=AF_INET;
   server_addr.sin_port=htons(port);
   server_addr.sin_addr.s_addr=inet_addr("127.0.0.1");

   int sockfd=socket(AF_INET,SOCK_STREAM,0);
   if(connect(sockfd,(struct sockaddr*)&server_addr,sizeof(server_addr)) <0)
   {
      printf("Error Connecting to Server...\n");
      exit(1);
   }
   char buffer[255];
   while(1)
   {
      bzero(buffer,255);
      fgets(buffer,255,stdin);
      write(sockfd,buffer,strlen(buffer));
      bzero(buffer,255);
      read(sockfd,buffer,255);
      capitalize(buffer);
      printf("Server: %s\n",buffer);

      if(strncasecmp(buffer,"exit",4)==0)
         break;
   }
   close(sockfd);
}
