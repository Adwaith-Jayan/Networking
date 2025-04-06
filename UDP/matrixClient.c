#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void main()
{
   int port=9680;
   struct sockaddr_in server_addr;

   bzero(&server_addr,sizeof(server_addr));

   server_addr.sin_family=AF_INET;
   server_addr.sin_port=htons(port);
   server_addr.sin_addr.s_addr=inet_addr("127.0.0.1");

   int sockfd=socket(AF_INET,SOCK_DGRAM,0);

   int A[10][10],B[10][10];
   int m,n,p,q;

   while(1)
   {
       printf("Dimensions of 1st matrix: ");
       scanf("%d%d",&m,&n);
       printf("Enter first matrix\n");
       for(int i=0;i<m;i++)
           for(int j=0;j<n;j++)
              scanf("%d",&A[i][j]);

       printf("Dimensions of 2nd matrix: ");
       scanf("%d%d",&p,&q);
       printf("Enter second matrix\n");
       for(int i=0;i<m;i++)
          for(int j=0;j<n;j++)
             scanf("%d",&B[i][j]);


       sendto(sockfd,&m,sizeof(m),0,(struct sockaddr*)&server_addr,sizeof(server_addr));
       sendto(sockfd,&n,sizeof(n),0,(struct sockaddr*)&server_addr,sizeof(server_addr));
       sendto(sockfd,&A,sizeof(A),0,(struct sockaddr*)&server_addr,sizeof(server_addr));
       sendto(sockfd,&p,sizeof(p),0,(struct sockaddr*)&server_addr,sizeof(server_addr));
       sendto(sockfd,&q,sizeof(q),0,(struct sockaddr*)&server_addr,sizeof(server_addr));
       sendto(sockfd,&B,sizeof(B),0,(struct sockaddr*)&server_addr,sizeof(server_addr));
   }
   close(sockfd);
}
