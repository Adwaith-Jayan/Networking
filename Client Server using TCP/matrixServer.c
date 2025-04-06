#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>

void matrixMultiply(int mat[][10], int m,int n)
{
   for(int i=0;i<m;i++)
   {
      for(int j=0;j<n;j++)
      {
          printf("%d  ",mat[i][j]);
      }
      printf("\n");
   }
}


void main()
{
    struct sockaddr_in server_addr,client_addr;
    int port=8500;
    memset(&server_addr,0,sizeof(server_addr));
    server_addr.sin_family=AF_INET;
    server_addr.sin_port=htons(port);
    server_addr.sin_addr.s_addr=INADDR_ANY;

    int sockfd=socket(AF_INET,SOCK_STREAM,0);

    bind(sockfd,(struct sockaddr*)&server_addr,sizeof(server_addr));
    listen(sockfd,5);

    socklen_t client_len=sizeof(client_addr);
    int newsockfd=accept(sockfd,(struct sockaddr*)&client_addr,&client_len);

    int mat[10][10];
    int row,col;
    while(1)
    {
       memset(mat,0,sizeof(mat));
       read(newsockfd,&row,sizeof(row));
       read(newsockfd,&col,sizeof(col));
       for(int i=0;i<row;i++)
          read(newsockfd,mat[i],col*sizeof(int));
       matrixMultiply(mat,row,col);
    }
    close(sockfd);
    close(newsockfd);
}
