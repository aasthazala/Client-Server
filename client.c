#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <signal.h>


int main(int argc, char *argv[]){
  char mess[255];
  int server, portNum, PID, n;
  struct sockaddr_in servAdd;     // Server's Socket Address
  
 if(argc != 3){                                                            // argument not equal to 3
    printf("Model Failed to call Port Number and IP Number\n", argv[0]);
    exit(0);
  }

  // AF_INET - address family that is used to designate the type of addresses that your socket can communicate with
  // SOCK_STREAM - TCP connection-based protocol.
  if ((server = socket(AF_INET, SOCK_STREAM, 0)) < 0){            // Socket could not be generated
     fprintf(stderr, "Socket not created.. Try Again\n");         
     exit(1);
  }

  
  servAdd.sin_family = AF_INET;                                    
  sscanf(argv[2], "%d", &portNum);
  servAdd.sin_port = htons((uint16_t)portNum);

  // AF_INET - src points to a character string containing an IPv4 network address in dotted-decimal format
  if(inet_pton(AF_INET, argv[1], &servAdd.sin_addr) < 0){                // Inet_pton() failed. 
  fprintf(stderr, " Failed to convert network address \n");
  exit(2);
}
// int connect(int sockfd, struct sockaddr *serv_addr, int addrlen);
// int sockfd - socket descriptor from the socket() call
// struct sockaddr *serv_addr - server’s sockaddr address, to be filled in
// int addrlen - length of the server’s sockaddr structure

 if(connect(server, (struct sockaddr *) &servAdd, sizeof(servAdd))<0){    // connect() failed
  fprintf(stderr, "Failed to establish a connection, Try again ... \n");
  exit(3);
 }

  read(server, mess, 255);
  fprintf(stderr, "Welcome, Type quit to exit: %s\n", mess);

  PID=fork();

  if(PID)                                          // Receiving Server Command
     while(1){					               
		 if(n=read(server, mess, 255)){           // Reading message from Server 
          mess[n]='\0';
          fprintf(stderr,"%s\n", mess);
          if(!strcasecmp(mess, "quit\n")){         // if message is quit, exit the socket
             exit(0);                              // strcasecmp - case-insensitive string comparison   
           }
         }
	 }                         
       

  if(!PID)                           // Client command send to Server
     while(1){
		 if(n=read(0, mess, 255)){
         mess[n]='\0';
         write(server, mess, strlen(mess)+1);  //  writing command 
         if(!strcasecmp(mess, "quit\n")){
            exit(0);
          }
      }
	 }
      
}
