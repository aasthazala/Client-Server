#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>



void serviceClient(int);

int main(int argc, char *argv[])
{  
  int sd, client, portNumber, status; 				// portNumber:The logical address of each application
  struct sockaddr_in servAdd;      				// Address of Client's Socket
  
  if(argc != 2)
  {

    printf("Calling the number by entering the port number: %s <Port Number>\n", argv[0]);
    exit(0);
  
  }
  
  if ((sd = socket(AF_INET, SOCK_STREAM, 0)) < 0)		//AF_INET:An address family that is used to designate the type of addresses that socket can communicate with.
								//SOCK_STREAM:TCP socket
  {

    fprintf(stderr, "OH NOOO! Can't able to create socket!\n");
    exit(1);

  }

  sd = socket(AF_INET, SOCK_STREAM, 0);
  servAdd.sin_family = AF_INET;
  servAdd.sin_addr.s_addr = htonl(INADDR_ANY);			//INADDR_ANY: allows our server to receive packets that have been targeted by any of the interfaces.
  sscanf(argv[1], "%d", &portNumber);
  servAdd.sin_port = htons((uint16_t)portNumber);		//sin_port:can be any integer > 5000
								//htons:convert values between host and network byte order
  
  bind(sd, (struct sockaddr *) &servAdd, sizeof(servAdd));	//bind:associates an address with the socket descriptor.
								//struct sockaddr:used to avoid compiler warnings.
  listen(sd, 5);

  while(1)
 {

    printf("Waiting for client to start the chat\n");
    client = accept(sd, NULL, NULL);
    printf("Connected!!! Now you can chat with client\n");
    
    if(!fork())

      serviceClient(client);

    close(client);
    waitpid(0, &status, WNOHANG);				//waitpid:Suspends the calling process until a child process ends or is stopped.
								//WNOHANG:Returns immediately if no child has exited.

  }

}

void serviceClient(int sd)
{
	char message[255];
        int n, pid;


        write(sd, "Start your conversation", 16);

	pid=fork();						//fork():Creates a new process, which is called child process.
	if(pid)                         			//Now server can read message from client side
       
	while(1)
	{

		   if(n=read(sd, message, 255))
		   {
			 message[n]='\0';
			 //fprintf(stderr,"%s", message);
			 if(!strcasecmp(message, "quit\n"))
			 {

			   //	        kill(pid, SIGTERM);
					exit(0);

			 }

			 dup2(sd, STDOUT_FILENO);			//STDOUT_FILENO:An integer file descriptor
			 fprintf(stderr,"%d", system(message));
		   }
	   }

	if(!pid)                      					//pid:An identification number
									//Server can send message to client side

	   while(1)
	   {

		  if(n=read(0, message, 255))
		  {

		  message[n]='\0';
		  write(sd, message, strlen(message)+1);
		  if(!strcasecmp(message, "quit\n"))		
									//strcasecmp:returns an integer less than, equal to, or greater than zero
	          {
			//     kill(getppid(), SIGTERM);		//getppid():returns the parent process ID of the calling process.
			       exit(0);
		   }

		}
	   }
}