#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>


void multiply(int A[][10],int m,int n,int B[][10],int p,int q)
{
   int c[m][q];
   for(int i=0;i<m;i++)
   {
       for(int j=0;j<q;j++)
       {
          c[i][j]=0;
          for(int k=0;k<n;k++)
             c[i][j]+=A[i][k]*B[k][j];
       }
   }
   printf("Matrix is\n");

   for(int i=0;i<m;i++)
   {
       for(int j=0;j<q;j++)
          printf("%d  ",c[i][j]);
       printf("\n");
   }

}

void main()
{
   int port=9680;
   struct sockaddr_in server_addr,client_addr;
   bzero(&server_addr,sizeof(server_addr));

   server_addr.sin_family=AF_INET;
   server_addr.sin_port=htons(port);
   server_addr.sin_addr.s_addr=INADDR_ANY;

   int sockfd=socket(AF_INET,SOCK_DGRAM,0);

   bind(sockfd,(struct sockaddr*)&server_addr,sizeof(server_addr));

   int matA[10][10], matB[10][10];
   int rowA,colA,rowB,colB;

   socklen_t client_len=sizeof(client_addr);
   while(1)
   {
      bzero(&rowA,sizeof(rowA));
      bzero(&colA,sizeof(colA));
      bzero(matA,sizeof(matA));
      recvfrom(sockfd,&rowA,sizeof(rowA),0,(struct sockaddr*)&client_addr,&client_len);
      recvfrom(sockfd,&colA,sizeof(colA),0,(struct sockaddr*)&client_addr,&client_len);
      recvfrom(sockfd,matA,sizeof(matA),0,(struct sockaddr*)&client_addr,&client_len);
      bzero(&rowB,sizeof(rowB));
      bzero(&colB,sizeof(colB));
      bzero(matB,sizeof(matB));
      recvfrom(sockfd,&rowB,sizeof(rowB),0,(struct sockaddr*)&client_addr,&client_len);
      recvfrom(sockfd,&colB,sizeof(colB),0,(struct sockaddr*)&client_addr,&client_len);
      recvfrom(sockfd,matB,sizeof(matB),0,(struct sockaddr*)&client_addr,&client_len);
      multiply(matA,rowA,colA,matB,rowB,colB);
   }
   close(sockfd);
}
