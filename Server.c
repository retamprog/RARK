//-----------------------------------The Server code------------------------------------//
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>

// the frequently used error function
// the canon event code which is going to recurr for many times .... 
int error(const char* msg){
   perror(msg);
   exit(1);
}

int main(int argc ,char* argv[]){
  // this is done to check whether the user has correctly given the right number of arguments and mainly to check whether the port number has been given or not...
  if(argc<2){
     printf("Port number is missing... Program terminated!!\n");
     exit(1);
  }
   int sockfd, newsockfd,portno,n;
   char buffer[256]; // this is the message buffer...
   struct sockaddr_in server_addr, cli_addr; 
   // the sockaddr_in contains gives us the internet address 
   socklen_t clilen; // 32 bit data type socklen_t stores the len of the client address
   
   sockfd = socket(AF_INET,SOCK_STREAM,0); 
   // socket(domain,type,protocol) -- protocol default 0 for TCP
   if(sockfd<0)
   error("error opening socket.");
   
   //bzero(); instead of this we are going to use memset function because the bzero function i     s depreciated and not used anymore and moreover it is not compatible with other OS.
    memset(&server_addr,0,sizeof(server_addr));
    portno = atoi(argv[1]);

    server_addr.sin_family=AF_INET;
    server_addr.sin_addr.s_addr=htonl(INADDR_ANY);
    server_addr.sin_port = htons(portno);
    // Here in Socket programming we have two types of datatypes for storing address in socket       APIS 
    // sockaddr --- > this is a generic protocol-independent struct and used as a general address container which is used a parameter in many socket api functions
    // struct sockaddr{
    //  sa_family_t sa_family;
    //  har sa_data[14]; // protocol specific address data
    // }
    //  
    //  sockadrr_in ---> specific struct designed for storing ipv4 address and port numbers  
    //  with clear named members..
    //
    //  struct sockaddr_in {
    //    sa_family_t sin_family;
    //    in_port_t sin_port;
    //    struct in_addr sin_addr;
    //    char sin_zero[8]; // for padding purposes;
    //  }
    //
    if(bind(sockfd,(struct sockaddr *)&server_addr,sizeof(server_addr)<0)){
	    error("the socket could'nt be allocated unique addr in the internet!!!!");
	    
     }
    
    // here the listen(file_des,the queue limit number) ---> here the file_des takes the s
    // socket file descriptor and the second parameter is an integer which takes the numb
    // er of clients that can connect to the server at one point.
    listen(sockfd,5); // for atmost 5 clients together can connect to the server..
    // this method listens for any incoming connection calls from the clients..
    clilen=sizeof(cli_addr);

    // when the connection gets accepted by the server listen() func , then a new file derciptor is created for the particular communication between the server and client.
    newsockfd=accept(sockfd,(struct sockaddr *)&cli_addr,&clilen);

    if(newsockfd<0){
      error("Error while accepting the connection...!!!");
    }

    while(1)
    {
       memset(buffer,0,256);
       n = read(newsockfd,buffer,256);
       if(n<0){
        error("Error on reading from client..!!");
       }
       printf("Client: %s\n",buffer);
       memset(buffer,0,256);
       fgets(buffer,256,stdin); // reading user input from the stdin  
       n = write(newsockfd,buffer,256);

       if(n<0){
        error("Error on writing to client...!!!");
       }

       int l = strncmp("Bye",buffer,3);
       if(l == 0){
         break;
       }
    }

     close(newsockfd);
     close(sockfd);
     return 0;

}



