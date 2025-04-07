#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <time.h>
#include <string.h>

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

   int frames=10;
   int window=4;
   int sent=0,send_pack=1,confirmed_pack=1;
   int ack;

   while(sent < frames)
   {
      for(int i=0;i<window;i++)
      {
         if(sent >=frames)
            break;
         if(send_pack <frames){
            printf("Sending Packet %d\n",send_pack);
            sendto(sockfd,&send_pack,sizeof(send_pack),0,(struct sockaddr*)&server_addr,sizeof(server_addr));
            send_pack++;
	 }
      }
      for(int i=0;i<window;i++)
      {
         if(sent >=frames)
             break;
         int n=recvfrom(sockfd,&ack,sizeof(ack),0,NULL,NULL);
         if(n>0)
         {
            if(ack==confirmed_pack)
            {
               printf("ACK received %d\n",ack);
               confirmed_pack++;
               sent++;
            }
         }
         else
         {
            printf("Time out for pakcet %d\n",confirmed_pack);
            printf("Resending from packet %d\n",confirmed_pack);
            send_pack=confirmed_pack;
         }

      }
   }
   close(sockfd);
}

