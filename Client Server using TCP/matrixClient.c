#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>

void main()
{
   int port=8500;
   struct sockaddr_in server_addr;
   memset(&server_addr,0,sizeof(server_addr));

   server_addr.sin_family=AF_INET;
   server_addr.sin_port=htons(port);
   server_addr.sin_addr.s_addr=inet_addr("127.0.0.1");

   int sockfd=socket(AF_INET,SOCK_STREAM,0);
   connect(sockfd,(struct sockaddr*)&server_addr,sizeof(server_addr));

   int mat[10][10];
   int row,col;
   while(1)
   {
      memset(mat,0,sizeof(mat));
      printf("Enter dimensions of the matrix: ");
      scanf("%d %d",&row,&col);
      printf("Enter matrix\n");
      for(int i=0;i<row;i++)
         for(int j=0;j<col;j++)
            scanf("%d",&mat[i][j]);
      write(sockfd,&row,sizeof(row));
      write(sockfd,&col,sizeof(col));
      for(int i=0;i<row;i++)
         write(sockfd,mat[i],col*sizeof(int));
   }
   close(sockfd);
}
