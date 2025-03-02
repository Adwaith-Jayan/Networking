#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

int factors[50];

int check_prime(char *buffer)
{
   int num = atoi(buffer);
   int prime = 1;
   memset(buffer, 0, 255);

   if (num < 2)
   {
      strcpy(buffer, "Not prime or composite");
   }
   else
   {
      for (int i = 2; i <= num / 2; i++)
      {
         if (num % i == 0)
         {
            prime = 0;
            break;
         }
      }
      if (prime)
         strcpy(buffer, "It is prime");
      else
         strcpy(buffer, "It is composite");
   }

   memset(factors, 0, sizeof(factors));

   if (!prime)
   {
      int k = 0;
      for (int i = 2; i <= num - 1; i++)
      {
         if (num % i == 0)
            factors[k++] = i;
      }
   }

   return prime;
}

int main()
{
   int sockfd;
   int port = 8000;
   struct sockaddr_in server_addr, client_addr;
   socklen_t cli_len = sizeof(client_addr);

   sockfd = socket(AF_INET, SOCK_DGRAM, 0);
   if (sockfd < 0)
   {
      perror("Socket creation failed");
      exit(EXIT_FAILURE);
   }

   memset(&server_addr, 0, sizeof(server_addr));
   server_addr.sin_family = AF_INET;
   server_addr.sin_addr.s_addr = INADDR_ANY;
   server_addr.sin_port = htons(port);

   if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
   {
      perror("Error binding");
      exit(EXIT_FAILURE);
   }

   char buffer[255];
   int arr[20];

   while (1)
   {
      memset(buffer, 0, sizeof(buffer));
      recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_addr, &cli_len);

      if (strncasecmp(buffer, "exit", 4) == 0)
      {
         printf("Client Exiting....\n");
         break;
      }

      printf("Client: %s", buffer);
      int prime = check_prime(buffer);
      printf("\n%s\n", buffer);

      memset(arr, 0, sizeof(arr));

      if (!prime)
      {
         printf("Factors are: ");
         int count = 0;
         for (int i = 0; i < 50 && factors[i] != 0; i++)
         {
            printf("%d ", factors[i]);
            arr[count++] = factors[i];
         }
         printf("\n");
      }

      sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&client_addr, cli_len);

      if (!prime)
      {
         sendto(sockfd, arr, sizeof(arr), 0, (struct sockaddr *)&client_addr, cli_len);
      }
   }

   close(sockfd);
   return 0;
}
