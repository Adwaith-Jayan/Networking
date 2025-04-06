#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

void main()
{
   int port=9680;
   struct sockaddr_in server_addr,client_addr;

   bzero(&server_addr,sizeof(server_addr));

   server_addr.sin_family=AF_INET;
   server_addr.sin_port=htons(9680);
   server_addr.sin_addr.s_addr=INADDR_ANY;

   int sockfd=socket(AF_INET,SOCK_DGRAM,0);

   bind(sockfd,(struct sockaddr*)&server_addr,sizeof(server_addr));

   char buffer[1024];
   socklen_t client_len=sizeof(client_addr);
   while(1)
   {
       bzero(buffer,sizeof(buffer));
       recvfrom(sockfd,buffer,sizeof(buffer),0,(struct sockaddr*)&client_addr,&client_len);
       printf("%s\n",buffer);
       bzero(buffer,sizeof(buffer));
       printf("Enter :");
       fgets(buffer,sizeof(buffer),stdin);
       sendto(sockfd,buffer,sizeof(buffer),0,(struct sockaddr*)&client_addr,client_len);

   }
   close(sockfd);
}
