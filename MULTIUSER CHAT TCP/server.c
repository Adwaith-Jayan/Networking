#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

#define maxclients 10

int clients[maxclients];
int n=0;
pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;


void send_to_all(char* msg,int current)
{
   pthread_mutex_lock(&mutex);
   for(int i=0;i<n;i++)
   {
      if(clients[i]!=current)
         write(clients[i],msg,strlen(msg));
   }
   pthread_mutex_unlock(&mutex);
}

void* handle_client(void* arg)
{
   int sock=*((int*)arg);
   char msg[1024];
   int len;
   while((len=recv(sock,msg,sizeof(msg),0))>0)
   {
       msg[len]='\0';
       printf("Broadcasting message : %s\n",msg);
       send_to_all(msg,sock);
   }
}

void  main()
{
   int port=9680;
   struct sockaddr_in server_addr,client_addr;
   bzero(&server_addr,sizeof(server_addr));
   socklen_t client_len=sizeof(client_addr);
   pthread_t t;

   server_addr.sin_family=AF_INET;
   server_addr.sin_port=htons(port);
   server_addr.sin_addr.s_addr=INADDR_ANY;

   int sockfd=socket(AF_INET,SOCK_STREAM,0);
   bind(sockfd,(struct sockaddr*)&server_addr,sizeof(server_addr));

  listen(sockfd,10);

  while(1)
  {
     int client_sock=accept(sockfd,(struct sockaddr*)&client_addr,&client_len);

     if(n< maxclients)
     {
        pthread_mutex_lock(&mutex);
        clients[n++]=client_sock;
        pthread_create(&t,NULL,handle_client,&client_sock);
        pthread_detach(t);
        pthread_mutex_unlock(&mutex);
     }
  }
  close(sockfd);
}
