/**
 * the client code execution pattern
 *  /Client server_ipaddr portno
 *  
 *  so there will be three args in the argv[]
 *
 */

//-------------------------------- The Client Code--------------------------------------// 
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>

void error(char * msg){
  perror(msg);
  exit(1);
}


int main(int argc,char * argv[]){
    if(argc<3)
    {
      printf("Give the correct args -- /Client server_ipaddr portno..!!!");
      exit(1);
    }	    
    int sockfd,portno,n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    // here the hostent struct is defined in the netdb.h file and is used to store info about the host and it protocols and the ipv4 addr.
    char buffer[256];

    portno=atoi(argv[2]);
    sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(sockfd<0)
      error("Error opening socket connection !!");
    
    server=gethostbyname(argv[1]);
    if(server== NULL){
      fprintf(stderr,"Error No such host");
    }    

    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    // Now here we are going to use a diff function called the memcpy instead of the bcopy
    // both of these functions are used to copy bytes of data from src to dest. Both of them are used copy raw memory blocks , such as copying addresses or data buffers.
    // bcopy is now depreciated and not a standard C library
    //
    // bcopy(const void *src,void *dest,size_t len);
    // Mostly used in Older legacy systems
    //
    // void *memcpy(void *dest,const void*src,size_t n);
    // returns the same pointer to the dest paraneter.
    // we are going to copy the address from the hostent server to serv_addr
    memcpy(&serv_addr.sin_addr.s_addr,server->h_addr,server->h_length);
    serv_addr.sin_port = htons(portno);
    if(connect(sockfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr))<0){
       error("Connection falied");
    }
    while(1){
     memset(buffer,0,256);
     fgets(buffer,256,stdin);
     n = write(sockfd,buffer,strlen(buffer));
     if(n<0)
       error("Error on writing!!!");     
    
     memset(buffer,0,256);
     n = read(sockfd,buffer,strlen(buffer));
     if(n<0){
       error("Error on reading!!");
     }
     printf("Server: %s\n",buffer);
     int l = strncmp("Bye",buffer,3);
     if(l==0)
      break;

     }

     close(sockfd);
     return 0;
}

