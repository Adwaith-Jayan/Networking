#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

void capitalize(char string[])
{
   int i=0;
   while(string[i]!='\0')
   {
      while(string[i]==' ' && string[i]!='\0')
      {
         i++;
      }
      if(string[i] >= 'a' && string[i] <='z')
         string[i]=string[i]-32;
      while(string[i]!=' ' && string[i]!='\0')
         i++;
   }
   printf("Client: %s\n",string);
}


void main()
{
   int port=8800;
   struct sockaddr_in server_addr,client_addr;
   socklen_t client_len;

   bzero((char*)&server_addr,sizeof(server_addr));
   server_addr.sin_family=AF_INET;
   server_addr.sin_addr.s_addr=INADDR_ANY;
   server_addr.sin_port=htons(port);

   int sockfd=socket(AF_INET,SOCK_STREAM,0);
   if(sockfd<0)
      printf("Error creating socket\n");

   if(bind(sockfd,(struct sockaddr*)&server_addr,sizeof(server_addr))<0)
      printf("Error Binding\n");

   listen(sockfd,5);

   client_len=sizeof(client_addr);
   int newsockfd=accept(sockfd,(struct sockaddr*)&client_addr,&client_len);

   char buffer[255];
   while(1)
   {
      bzero(buffer,255);
      read(newsockfd,buffer,255);
      capitalize(buffer);
      bzero(buffer,255);
      fgets(buffer,255,stdin);
      write(newsockfd,buffer,strlen(buffer));

      if(strncasecmp(buffer,"exit",4)==0)
         break;
   }
   close(sockfd);
   close(sockfd);
}
