#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

void main()
{
   int sockfd;
   int port=8000;

   struct sockaddr_in server_addr;
   sockfd=socket(AF_INET,SOCK_DGRAM,0);
   bzero((char*)&server_addr,sizeof(server_addr));
   server_addr.sin_family=AF_INET;
   server_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
   server_addr.sin_port=htons(port);

   char buffer[255];
   int arr[20];
   while(1)
   {
      bzero(buffer,255);
      printf("Enter no: ");
      fgets(buffer,255,stdin);
      if(strncasecmp(buffer,"exit",4)==0)
         break;
      sendto(sockfd,buffer,255,0,(struct sockaddr*)&server_addr,sizeof(server_addr));
      bzero(buffer,255);
      bzero((char*)arr,20);
      recvfrom(sockfd,buffer,255,0,NULL,NULL);
      printf("Server: %s\n",buffer);
      bzero((char*)&arr,20);
      recvfrom(sockfd,arr,20,0,NULL,NULL);
      int n=sizeof(arr)/sizeof(arr[0]);
      printf("Server:\n");
      printf("Factors are : ");
      for(int i=0;i<n;i++)
         printf("%d",arr[0]);

   }
   close(sockfd);
}
