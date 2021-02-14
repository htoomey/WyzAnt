/*				newClient.C


Modified from Tannenbaum 5th edition
Harold Toomey
9/17/19

*/



/* This is the Client program which will connect to the server program.
  The server program will open a file called "test" and will send the 
  the contents of the file in one message over to the client
  the buflen is currently set to the size of the message (18) which
  can accomodate the file in its entirety.

To compile the program type: gcc -o client newClient.c

To run the program type:
client sender-hostname receiver-port# 

*/

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

main(int argc,char *argv[])
{
  int s;   /* socket descripter */
  int c, bytes;
  struct sockaddr_in channel;

  struct hostent *h, *gethostbyname();
  int buflen = 20;
  char buf[20];

  if (argc !=3)
     fatal(" usage: client servername port#");

  /* create socket */
  s = socket(AF_INET, SOCK_STREAM, 0);
  if(s < 0) {
    perror("Opening stream socket");
    exit(1);
  }
  
  /* Create server socket address using command line arguments */

  h = gethostbyname(argv[1]);
  if(h == 0) {
    printf("%s: unknown host\n",argv[1]);
    exit(2);
  }
  memset(&channel, 0, sizeof(channel));
  channel.sin_family = AF_INET;
  memcpy(&channel.sin_addr.s_addr, h->h_addr, h->h_length);
  channel.sin_port = htons(atoi(argv[2]));   /*server port # */

  /* connect socket using name specified at command line */
   c = connect(s, (struct sockaddr *)&channel, sizeof(channel));
   if (c < 0)
      fatal("connect failed");

      while (bytes = read(s, buf, buflen))
      {
          printf("Received: %d\n%s\n",bytes, buf); 
      }
   close(s);
}

/*  Function to print error message if any socket calls fail
*/
fatal(char *string)
{ 
   printf("in function");
   printf("%s\n", string);
   exit(0);
}

