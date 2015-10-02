#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>

#define MAX 10000
typedef struct {
    char *name;
    char *value;
} ENTRY;

ENTRY entry[MAX];

main(int argc, char *argv[]) 
{
  int i, m, r;
  char cwd[128];

  m = getinputs();    // get user inputs name=value into entry[ ]
  getcwd(cwd, 128);   // get CWD pathname

  printf("Content-type: text/html\n\n");
  printf("<p>pid=%d uid=%d cwd=%s\n", getpid(), getuid(), cwd);

  printf("<H1>Echo Your Inputs</H1>");
  printf("You submitted the following name/value pairs:<p>");
 
  for(i=0; i <= m; i++)
     printf("%s = %s<p>", entry[i].name, entry[i].value);
  printf("<p>");


  /*****************************************************************
   Write YOUR C code here to processs the command
         mkdir dirname
         rmdir dirname
         rm    filename
         cat   filename
         cp    file1 file2
         ls    [dirname] <== ls CWD if no dirname
  *****************************************************************/
 
  // create a FORM webpage for user to submit again 
  printf("</title>");
  printf("</head>");
  printf("<body bgcolor=\"#c49561\" link=\"#330033\" leftmargin=8 topmargin=8");
  printf("<p>------------------ DO IT AGAIN ----------------\n");
  
  printf("<FORM METHOD=\"POST\" ACTION=\"http://cs360.eecs.wsu.edu/~juel/cgi-bin/mycgi\">");
  
  printf("Enter command : <INPUT NAME=\"command\"> <P>");
  printf("Enter filename1: <INPUT NAME=\"filename1\"> <P>");
  printf("Enter filename2: <INPUT NAME=\"filename2\"> <P>");
  printf("Submit command: <INPUT TYPE=\"submit\" VALUE=\"Click to Submit\"><P>");
  printf("</form>");
  printf("------------------------------------------------<p>");

  printf("</body>");
  printf("</html>");
}

void myls(char* path, char cwd[]){ // Work?
	struct stat stats;
	struct info *infom;
	if(strlen(path) == 0) {
		strcpy(path, ".");
	}
	DIR *dp = opendir(path);
	//opens a DIR (for R/W), and
	if(dp) {
		struct dirent *ep = readdir(dp);
		//returns ep pointing at the next entry of an opened DIR
		while(ep != NULL){
			lstat(ep->d_name, &stats);
			printf("%s \n", ep->d_name);
			ep = readdir(dp);
		}
	} else {
		printf("Could not open '%s'\n", path);
	}
}
