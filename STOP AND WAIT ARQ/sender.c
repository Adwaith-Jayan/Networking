#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <time.h>

#define timeout_sec 3
void main()
{
   int port=9680;
   struct sockaddr_in server_addr;
   bzero(&server_addr,sizeof(server_addr));

   server_addr.sin_family=AF_INET;
   server_addr.sin_port=htons(port);
   server_addr.sin_addr.s_addr=inet_addr("127.0.0.1");

   int sockfd=socket(AF_INET,SOCK_DGRAM,0);

   struct timeval timeout={timeout_sec,0};
   setsockopt(sockfd,SOL_SOCKET,SO_RCVTIMEO,&timeout,sizeof(timeout));

   int ack;
   int frames=10,sent=0, send_frame=1;

   while(sent<frames)
   {
       printf("Sending Packet %d \n",send_frame);
       sendto(sockfd,&send_frame,sizeof(send_frame),0,(struct sockaddr*)&server_addr,sizeof(server_addr));
       int n=recvfrom(sockfd,&ack,sizeof(ack),0,NULL,NULL);
       if(n >0)
       {
          printf("Acknowledgment Received: %d\n",ack);
          if(ack==send_frame)
          {
              sent++;
              send_frame++;
              sleep(1);
          }
          else
          {
             printf("Wrong ACK ... Resending Packet....\n");
          }
       }
       else
       {
          printf("Time out resending packet...\n");
       }
   }
   close(sockfd);
}
