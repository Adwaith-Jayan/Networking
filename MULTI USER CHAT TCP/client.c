#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

void* recv_msg(void* arg)
{
   int sockfd=*((int*)arg);
   char msg[1024];
   int len;
   while((len=recv(sockfd,msg,sizeof(msg),0)) >0 )
   {
       msg[len]='\0';
       printf("%s\n",msg);
       fflush(stdout);
   }

}

void main()
{
   int port=9680;
   struct sockaddr_in server_addr;
   bzero(&server_addr,sizeof(server_addr));

   pthread_t t2;

   server_addr.sin_family=AF_INET;
   server_addr.sin_port=htons(port);
   server_addr.sin_addr.s_addr=inet_addr("127.0.0.1");

   int sockfd=socket(AF_INET,SOCK_STREAM,0);

   connect(sockfd,(struct sockaddr*)&server_addr,sizeof(server_addr));

   pthread_create(&t2,NULL,recv_msg,&sockfd);

   char name[1024];
   printf("Enter username: ");
   fgets(name,sizeof(name),stdin);
   name[strcspn(name,"\n")]='\0';

   char buffer[1024];
   char m[1024];
   while(1)
   {
       bzero(buffer,sizeof(buffer));
       fgets(buffer,sizeof(buffer),stdin);
       bzero(m,sizeof(m));
       strcpy(m,name);
       strcat(m,": ");
       strcat(m,buffer);
       write(sockfd,m,strlen(m));
   }
   close(sockfd);

}
