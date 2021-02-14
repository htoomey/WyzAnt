/*				Server.c

Modified from Tannenbaum 5th edition
Harold Toomey
9/17/2019
*/

/* This is the server program. It opens a socket at a fixed port # 50199.
   The client will establish a connection to this port# and the specified
   node name.  The server program will open a file called "test" and send
   it to the client.

   To compile type: gcc -o server newServer.c
 
   NOTE:  The server program MUST be started first so that the client program
          can connect to it.

   To run the program type:  server  

Note that the server always uses port 50199 as defined above.  This number
 can be changed to any number above 50000 - these are open ports to external
 connections at CSUEB.
*/
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/fcntl.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <errno.h>
#include <sys/time.h>
#include <stdlib.h>
#include <strings.h>

#define TRUE 1
#define SERVER_PORT 50199


main()
{
  int s, b, length, fd,l, sa, bytes, on = 1; 
  int i=0;
  int more = 1;
  struct sockaddr_in channel;
  int buflen = 19;
  char buf[19];

  /* create socket */
  s = socket(AF_INET, SOCK_STREAM, 0);
  if (s < 0) 
    fatal("Error opening stream socket");

  setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (char *) &on, sizeof(on));

  /* name socket using wildcards */
  channel.sin_family = AF_INET;
  channel.sin_addr.s_addr = INADDR_ANY;
  channel.sin_port = htons(SERVER_PORT);
   b = bind(s, (struct sockaddr *)&channel, sizeof(channel)); 
   if (b < 0)
      fatal("bind failed");


  length = sizeof(channel);

  if(getsockname(s, (struct sockaddr *)&channel, &length)){
    perror("getting socket name");
    exit(1);
  }
  /* find out assigned port number and print out */

  printf("Socket has port #%d\n",ntohs(channel.sin_port));


  /* Accept connections from the transmitter queue size set to 5 */
   l = listen(s, 5);
   if (l < 0)
      fatal("listen failed");

   sa = accept(s,0,0);

   if(sa  < 0)
     fatal("accept failed");

   /* get and return the file */
   fd = open("test",O_RDONLY); 
   if (fd < 0)
       fatal("file open error");
   
   for (i = 0; i < 5; i++)
   {
        bytes = read(fd, buf, buflen); /* read from file */
        write(sa, buf, bytes);        /* write bytes to socket */
        sleep(1);
   }
   close(fd);                       /* close file */
   close(sa);                       /* close socket */
}
/*
 * function for printing errors if any occur on socket calls and reads
 */
fatal(char *string)
{
   printf("%s\n",string);
   exit(1);
}
