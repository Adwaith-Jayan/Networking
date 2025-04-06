#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

void main()
{
   int port=9680;
   struct sockaddr_in server_addr;

   bzero(&server_addr,sizeof(server_addr));
   printf("%ld\n",sizeof(server_addr));
   server_addr.sin_family=AF_INET;
   server_addr.sin_port=htons(port);
   server_addr.sin_addr.s_addr=inet_addr("127.0.0.1");

   int sockfd=socket(AF_INET,SOCK_DGRAM,0);

   char buffer[1024];

   printf("%ld\n",sizeof(server_addr));

   while(1)
   {
       printf("Enter message: ");
       bzero(buffer,sizeof(buffer));
       fgets(buffer,sizeof(buffer),stdin);
       sendto(sockfd,buffer,sizeof(buffer),0,(struct sockaddr*)&server_addr,sizeof(server_addr));
       bzero(buffer,sizeof(buffer));
       recvfrom(sockfd,buffer,sizeof(buffer),0,NULL,NULL);
       printf("%s\n",buffer);
   }
   close(sockfd);
}
