/*				web.c
Harold Toomey
7 May 2020
*/

/**************************************************************************
  This simple HTTP Web browser connects to a Web server.
  The web client will make a GET request.
  The Web Server will send a header and HTML text.
  The HTML is then filtered for text and links.

1) To edit, use your favorite code editor.

2) To compile the program type:
% gcc -o web web.c

3) To run the program type:
% ./web webServerName receiverPort#
% ./web csweb01.csueastbay.edu 80

***************************************************************************/

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>			// Needed for socket read(), write(), and close()

#define TRUE 1
#define FALSE 0
#define BUF_LEN 2048        // URL = 2048 chars, includes 18 chars for "GET "
#define LINE_LEN 200
#define URL_LEN 200     	// URL = 2048 max chars
#define MAX_URLS 10
#define DEBUG TRUE			// Set to TRUE to see debug output, otherwise set to FALSE

// Function Prototypes
int fatal(char *string);	// Removed CygWin compiler warnings
void init_buffer(char buffer[BUF_LEN]);
void init_urls(char url[MAX_URLS][URL_LEN]);
void find_urls(char buf[BUF_LEN], char url[MAX_URLS][URL_LEN]);
int save_url(char line[LINE_LEN], char url[MAX_URLS][URL_LEN], int urlCount);
void print_urls(char url[MAX_URLS][URL_LEN]);
void print_clean_page(char buf[BUF_LEN], char url[MAX_URLS][URL_LEN]);
void print_clean_line(char line[LINE_LEN], char url[MAX_URLS][URL_LEN]);


/****************************************************************************************
main
*****************************************************************************************/

int main(int argc, char *argv[])
{
  int s;  					// socket descripter
  int c, bytes;
  int i;
  struct sockaddr_in channel;

  struct hostent *h, *gethostbyname();
  int buflen = BUF_LEN;
  char buf[BUF_LEN];
  char serverName[URL_LEN];

  char urlNum;
  char url[MAX_URLS][URL_LEN];	// Will store a fixed number of links

  char currentServer[URL_LEN];
  char currentDir[URL_LEN];
  char currentPath[URL_LEN];
  char currentHomePage[URL_LEN];
  char currentFullURL[URL_LEN];
  int currentPort = 80;
  

  if (argc !=3)
     fatal(" usage: web webServerName receiverPort#");

  /* Create a socket */
  s = socket(AF_INET, SOCK_STREAM, 0);
  if(s < 0) {
    perror("Opening stream socket");
    exit(1);
  }

  // Do Once
  currentPort = atoi(argv[2]);
  strcpy(currentServer, argv[1]);
  strcat(currentDir, "/~td4679/Pgm4/");
  strcpy(currentPath, currentServer);
     strcat(currentPath, currentDir);
  strcpy(currentHomePage, "start");
    
  strcpy(currentFullURL, "http://");
  strcat(currentFullURL, currentPath);
  strcat(currentFullURL, currentHomePage);
  strcat(currentFullURL, "\n\0");
  
#if DEBUG
  printf("currentServer = \"%s\"\n", currentServer);
  printf("currentDir = \"%s\"\n", currentDir);
  printf("currentPath = \"%s\"\n", currentPath);
  printf("currentHomePage = \"%s\"\n", currentHomePage);
  printf("currentFullURL = \"%s\"\n", currentFullURL);
#endif

  init_urls(url);		  // Zero out URL list
  
  strcpy(&url[0][0], currentFullURL);
  strcpy(&url[0][strlen(&url[0][0])+1], currentHomePage);
#if DEBUG
  printf("URL = %s", url[0]);
  printf("Des = %s", &url[0][strlen(&url[0][0])+1]);
  printf("Port = %d", currentPort);
#endif

return(1);
  
  

  // Zero out buffer
  init_buffer(buf);

  urlNum = 1;

  do
  {
	  // Prompt for URL
	  printf("\nSelect URL:\n");
	  printf("  0 - EXIT\n");
	  for (i=0; url[i][0] != '\0'; i++)
	  	printf("  %d - %s\n", i+1, currentHomePage);
	  printf("\n");
	  urlNum = getchar();
	  if (urlNum == '0')
		return(0);

	  // Set current server name. Remofe "http://" prefix and dir path suffix.
  	  strcpy(currentServer, &url[urlNum-1][strlen("http://")]);
	  for (i=0; (i <= strlen(currentserver) && (currentserver[urlNum-1][i] != '/'), i++)
	     currentServer[i] = '\0';
	  
	  // Set home page description
      strcpy(currentHomePage, &url[urlNum-1][strlen(url[urlNum-1])+1]);
  	  
	  // Set currentPath for short URLs
	  // Algorithm: if(no '/', then shortURL)
	  strcpy(currentPath, url[urlNum-1]);
	  for (i = strlen(currentPath)-1; currentPath[i] != '/'; i--)
		 currentPath[i] = '\0';
#if DEBUG
	  printf("currentPath = \"%s\"\n", currentPath);
#endif


	  /* Create server socket address using command line arguments */
	  h = gethostbyname(currentServer);
	  if(h == 0) {
		printf("%s: unknown host\n", currentServer);
		exit(2);
	  }

	  // Build the GET request for the web server
	  strcpy(buf, "GET ");
	  strcat(buf, serverName);
	  strcat(buf, " HTTP/1.0\r\n\r\n\0");
#if DEBUG
	  printf("buf = \"%s\"\n\n", buf);
#endif

	  memset(&channel, 0, sizeof(channel));
	  channel.sin_family = AF_INET;
	  memcpy(&channel.sin_addr.s_addr, h->h_addr, h->h_length);
	  channel.sin_port = htons(atoi(argv[2]));   /*server port # */

	  /* connect socket using name specified at command line */
	   c = connect(s, (struct sockaddr *)&channel, sizeof(channel));
	   if (c < 0)
		  fatal("connect failed");

		  //while (bytes = read(s, buf, buflen))
		  //{
		  //  printf("Received: %d\n%s\n",bytes, buf);
		  //}

	   // Send the GET request to the web server
	   write(s, buf, strlen(buf)+1);

	   // Receive the HTML web page response from the web server
	   // NOTE: Only reads the first BUF_LEN characters
	   bytes = read(s, buf, buflen);

	   // Done
	   close(s);

	   // Search for all links in the HTML
	//   find_urls(buf, url);

	   // Print web page in HTML with tags still visible 70pts
	#if DEBUG
	   printf("=====================================================\n");
	   printf("%s", buf);
	#endif

	   // Retrieves one page with html tags not visible 80pts
	   print_clean_page(buf, url);

	   // DEBUG: Show all links found
	   //print_urls(url);


	  // Zero out buffer
	  init_buffer(buf);
	  	  
	  // Zero out URL list
	  init_urls(url);
  
  } while (urlNum != 0);  

   return(0);
}



/****************************************************************************************
  Function to print error message if any socket calls fail
*****************************************************************************************/
int fatal(char *string)
{
   printf("in function");
   printf("%s\n", string);
   exit(0);
}


/****************************************************************************************
  Initializes the buffer to NULL
*****************************************************************************************/
void init_buffer(char buffer[BUF_LEN])
{
	int i;
	
    for (i=0; i < BUF_LEN; i++)
	   buffer[i] = '\0';
}


/****************************************************************************************
   Initializes the list of URLs to NULL
*****************************************************************************************/
void init_urls(char url[MAX_URLS][URL_LEN])
{
	int i, j;

    for (i=0; i < MAX_URLS; i++)
       for (j=0; j < URL_LEN; j++)
		  url[i][j] = '\0';
}


/****************************************************************************************
   Locate all links on the web page
*****************************************************************************************/
void find_urls(char buf[BUF_LEN], char url[MAX_URLS][URL_LEN])
{
   int urlCount = 0;
   int urlLength;
   int urlTextLength;
   int urlStart;
   int urlTextStart;
   int bufferLength = strlen(buf);
   int i;
   
#if DEBUG
   // printf("strlen(buf) = %d\n", strlen(buf));
#endif

   // Search for URLs: <a href="camp.html">Camping</a>
   for (i=0; i < bufferLength; i++)
   {
      if (strncasecmp(&buf[i], "<a href=\"", 9) == 0)
      {
		 // Save URL
         urlStart = i+9;
         for (urlLength=0; buf[urlStart + urlLength] != '\"'; urlLength++);
         strncpy(&url[urlCount][0], &buf[urlStart], urlLength);
         url[urlCount][urlLength] = '\0';
         
         // Save URL Text right after the URL itself
         //  FORMAT: <URL> + \0 + <URL Text> + \0
         urlTextStart = urlStart + urlLength + 2;
         for (urlTextLength=0; buf[urlTextStart + urlTextLength] != '<'; urlTextLength++);
         strncpy(&url[urlCount][urlLength+1], &buf[urlTextStart], urlTextLength);
         url[urlCount][urlLength + urlTextLength + 1] = '\0';

         urlCount++;         
      }
   }
}


/****************************************************************************************
   Locate url in a line of HTML
****************************************************************************************/
int save_url(char line[LINE_LEN], char url[MAX_URLS][URL_LEN], int urlCount)
{
   int urlLength;
   int urlTextLength;
   int urlStart;
   int urlTextStart;
   int lineLength = strlen(line);
   int i;
   
#if DEBUG
   // printf("strlen(line) = %d\n", strlen(line));
#endif

   // Search for URL in line: <a href="camp.html">Camping</a>
   for (i=0; i < lineLength; i++)
   {
      if (strncasecmp(&line[i], "<a href=\"", 9) == 0)
      {
		 // Save URL
         urlStart = i+9;
         for (urlLength=0; line[urlStart + urlLength] != '\"'; urlLength++);
         strncpy(&url[urlCount][0], &line[urlStart], urlLength);
         url[urlCount][urlLength] = '\0';	// Terminate string before storing
         
         // Save URL Text right after the URL itself
         //  FORMAT: <URL> + \0 + <URL Text> + \0
         urlTextStart = urlStart + urlLength + 2;
         for (urlTextLength=0; line[urlTextStart + urlTextLength] != '<'; urlTextLength++);
         strncpy(&url[urlCount][urlLength+1], &line[urlTextStart], urlTextLength);
         url[urlCount][urlLength + urlTextLength + 1] = '\0';	// Terminate string before storing

         urlCount++;         
      }
   }
   return(urlCount);
}


/****************************************************************************************
  Print list of URLs (for DEBUGGING)
*****************************************************************************************/
void print_urls(char url[MAX_URLS][URL_LEN])
{
	int i;
	
    printf("Web Links:\n");
    for (i=0; (i < MAX_URLS) && (url[i][0] != '\0'); i++)
    {
	   // printf("%d) %s\t\"%s\"\n", i+1, &url[i][0], &url[i][strlen(&url[i][0])+1]);
   	   printf("%d) %s\n", i+1, &url[i][strlen(&url[i][0])+1]);
	}
	printf("\n");
}


/****************************************************************************************
   Print cleaned up page
*****************************************************************************************/
void print_clean_page(char buf[BUF_LEN], char url[MAX_URLS][URL_LEN])
{
   int htmlStart = 0;
   int htmlLineEnd = 0;
   char line[200];
   int lineIndex;
   int found=FALSE;
   int i;

   // Skip past header
   // Search for <CR/LF> <CR/LF>
   for (i=0; i < BUF_LEN; i++)
      if (strncmp(&buf[i], "\r\n\r\n", 4) == 0)
         htmlStart = i+4;
   
#if DEBUG
   // Print web page in HTML without header, but with HTML tags
   printf("=====================================================");
   printf("\n%s\n", &buf[htmlStart]);
   printf("=====================================================\n");
#endif

   // Print web page in HTML without header, and without HTML tags
   // Parse HTML 1 line at a time
   while (htmlStart < strlen(buf))
   {
	   // Grab a line.  They all end with a <CR/LF>.
	   for (i=htmlStart; (i < htmlStart + LINE_LEN) && (found == FALSE); i++)
	   {
		  if (strncmp(&buf[i], "\n", 1) == 0)
		  {
			 htmlLineEnd = i;
			 found = TRUE;
		  }
	   }
	   strncpy(line, &buf[htmlStart], htmlLineEnd - htmlStart);
	   line[htmlLineEnd - htmlStart] = '\0';	// Terminate line before processing
	   htmlStart = htmlLineEnd + 1;
#if DEBUG
	   //printf("Line: \"%s\"\n", line);
#endif
       print_clean_line(line, url);
	   found = FALSE;
	}
#if DEBUG
     printf("=====================================================\n");   
#endif
}


/****************************************************************************************
   Print a line of HTML with all tags removed
*****************************************************************************************/
void print_clean_line(char line[LINE_LEN], char url[MAX_URLS][URL_LEN])
{
   int lineIndex = 0;
   static int urlCount;
   int brFound;
   int i, j;

   // Skip past <html>, <body>, and <ul> tags
   if (strncmp(line, "<html>", 6) == 0)
   {
      lineIndex = 0;
      urlCount = 0;
      return;
   }
   
   if (strncmp(line, "<body>", 6) == 0)
      return;
      
   if (strncmp(line, "</body>", 7) == 0)
      return;

   if (strncmp(line, "</html>", 7) == 0)
      return;

   if (strncmp(line, "<ul>", 4) == 0)
      return;
   
   // Process line with single </ul> tag
   if (strncmp(line, "</ul>", 5) == 0)
   {
      printf("\n");
      return;
   }
   
   // Process line with <p> tag
   if (strncmp(line, "<p>", 3) == 0)
   {
      printf("\n");
      if (strlen(line) > 3)
      {
         lineIndex += 3;
         printf("%s\n", &line[lineIndex]);
      }
      return;
   }   

   // Process image ALT text within <img SRC> tag
   if (strncmp(line, "<img SRC=", 9) == 0)
   {
 	  // Search for internal ALT= tag, then print
	  for (i=0; i < LINE_LEN; i++)
		 if (strncmp(&line[i], "ALT=", 4) == 0)
		 {
		    lineIndex = i+5;		// Offset for tag + "
            for (j=lineIndex; j < LINE_LEN; j++)
               if (line[j] == '\"')
                  line[j] = '\0';	// Terminate string before printing

		    printf("%s\n", &line[lineIndex]);   
		    lineIndex += 4 + strlen(&line[lineIndex]);
		 }
      return;
   }
   
   // NOTE: This code for handling <br> could use some cleanup
   // Process line with <li><a href>= tags, then print URL menu item
   if ((strncmp(line, "<li><a href=", 12) == 0) || (strncmp(line, "<li> <a href=", 13) == 0))
   {
      urlCount = save_url(line, url, urlCount);
      printf("%d) %s\n", urlCount, &url[urlCount-1][strlen(&url[urlCount-1][0])+1]);
      return;
   }
   
   // Process line with 1 or 2 <br> tags at the front
   if (strncmp(line, "<br><br>", 8) == 0)
   {
      printf("\n\n");
      return;
   } 
   else if (strncmp(line, "<br>", 4) == 0)
   {
      if (strlen(line) <= 4)
         printf("\n");
      else
         printf("\n%s", &line[4]);
      return;
   }
   
   // Process line with multiple <br> tags
   brFound = FALSE;
   for (i=0; i < LINE_LEN; i++)
   {
      if (strncmp(&line[i], "<br>", 4) == 0)
      {
         brFound = TRUE;
         if ((i == 0) && strlen(&line[lineIndex]) == 4)
            return;
         else
            line[i] = '\0';		// Terminate string before printing
         printf("%s\n", &line[lineIndex]);
         i += 3;
         lineIndex = i+1;
      }
   }
   
   // Print text after checking for tags
   printf("%s\n", &line[lineIndex]);		// No tags left so print text
   return;   
}

/***************************************************************************************/
