#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int arr[20];

int main()
{
   int sockfd;
   int port = 8000;
   struct sockaddr_in server_addr;

   sockfd = socket(AF_INET, SOCK_DGRAM, 0);
   if (sockfd < 0)
   {
      perror("Socket creation failed");
      exit(EXIT_FAILURE);
   }

   memset(&server_addr, 0, sizeof(server_addr));
   server_addr.sin_family = AF_INET;
   server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
   server_addr.sin_port = htons(port);

   char buffer[255];

   while (1)
   {
      memset(buffer, 0, sizeof(buffer));
      printf("Enter number (or type 'exit' to quit): ");
      fgets(buffer, sizeof(buffer), stdin);

      buffer[strcspn(buffer, "\n")] = '\0';

      if (strncasecmp(buffer, "exit", 4) == 0)
         break;

      sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));

      memset(buffer, 0, sizeof(buffer));
      recvfrom(sockfd, buffer, sizeof(buffer), 0, NULL, NULL);
      printf("Server: %s\n", buffer);

      if (strcasecmp(buffer, "It is prime") != 0 && strcasecmp(buffer, "Not prime or composite") != 0)
      {
         memset(arr, 0, sizeof(arr));
         recvfrom(sockfd, arr, sizeof(arr), 0, NULL, NULL);
         printf("Factors: ");
         for (int i = 0; i < 20; i++)
         {
            if (arr[i] != 0)
               printf("%d ", arr[i]);
         }
         printf("\n");
      }
   }

   close(sockfd);
   return 0;
}
