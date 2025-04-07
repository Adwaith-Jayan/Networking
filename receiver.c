#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <time.h>
#include <string.h>

void main()
{
   int port=9680;
   struct sockaddr_in server_addr,client_addr;
   socklen_t client_len=sizeof(client_addr);

   bzero(&server_addr,sizeof(server_addr));

   server_addr.sin_family=AF_INET;
   server_addr.sin_port=htons(port);
   server_addr.sin_addr.s_addr=INADDR_ANY;

   int sockfd=socket(AF_INET,SOCK_DGRAM,0);
   bind(sockfd,(struct sockaddr*)&server_addr,sizeof(server_addr));

   int ack=1;
   int frame=10,recved=0,rec_packet;

   while(recved<frame)
   {
      recvfrom(sockfd,&rec_packet,sizeof(rec_packet),0,(struct sockaddr*)&client_addr,&client_len);
      printf("Packet Received %d\n",rec_packet);
      if(rand() % 10 <3)
         continue;
      if(rec_packet==ack)
      {
          printf("Sending ACK %d\n",ack);
          sendto(sockfd,&ack,sizeof(ack),0,(struct sockaddr*)&client_addr,client_len);
          ack++;
          recved++;
      }
   }
   close(sockfd);
}


