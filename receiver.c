#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>

void main()
{
   int port = 9680;
   struct sockaddr_in server_addr, client_addr;
   bzero(&server_addr, sizeof(server_addr));
   socklen_t client_len = sizeof(client_addr);

   server_addr.sin_family = AF_INET;
   server_addr.sin_port = htons(port);
   server_addr.sin_addr.s_addr = INADDR_ANY;

   int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
   bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));

   int frame = 10;
   int window = 4;
   int ack = 1;
   int recvd = 0;
   int recvd_pack;
   int confirmed_recvd = 1;
   int expected = 1;
   while (recvd < frame)
   {
      for (int i = 0; i < window; i++)
      {
         if (recvd >= frame)
            break;
         recvfrom(sockfd, &recvd_pack, sizeof(recvd_pack), 0, (struct sockaddr *)&client_addr, &client_len);
         printf("Received  Packet %d\n", recvd_pack);
         expected = recvd_pack;
      }
      for (int i = 0; i < window; i++)
      {
         ack = expected - window - 1;
         if (recvd >= frame)
            break;
         if (rand() % 10 < 3)
            continue;
         sendto(sockfd, &ack, sizeof(ack), 0, (struct sockaddr *)&client_addr, client_len);
         printf("Sending ACK %d\n", ack);
         ack++;
         recvd++;
      }
   }
   close(sockfd);
}
