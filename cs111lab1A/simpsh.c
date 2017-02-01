#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>

void execute(char **params)
{
   pid_t  pid;
  int    status;

  if ((pid = fork()) < 0) {    
    printf("*** ERROR: forking child process failed\n");
    exit(1);
  }
  else if (pid == 0) {         
    if (execvp(params[3], &params[3]) < 0) { 
      printf("*** ERROR: exec failed\n");
      exit(1);
    }
  }
  // else {                           
  //while (wait(&status) != pid)   
  //  ;
  // }
}

int main (int argc, char ** argv)
{

  int c = 0;
  int verbose_flag = 0;
  struct option longopts[] =
    {
      {"rdonly", required_argument, 0, 'a' },
      {"wronly", required_argument, 0,'b'},
      {"verbose", no_argument, &verbose_flag, 1},
      {"command", no_argument, 0, 'm'},
      {0, 0, 0, 0}    
    };

  char* inputfile = NULL;
  char* writefile = NULL;
  int fileds[argc];
  int numfiles = 0;
  while ((c = getopt_long (argc, argv, "a:b:m", longopts, 0)) != -1)
    {
      
      switch (c)
	{
	case 'a':
	  {
	    inputfile = optarg;
	    int ifd = open(inputfile, O_RDONLY);
	    fileds[numfiles] = ifd;
	    numfiles = numfiles + 1;
	    if (verbose_flag == 1)
	      {
		write(1, "--rdonly ", 9);
		int length = strlen(inputfile);
		write(1, inputfile, length);
		write(1, "\n", 1);
	      }
	    break;
	  }
	case 'b':
	  {
	    writefile = optarg;
	    int ofd = open(writefile, O_WRONLY);
	    fileds[numfiles] = ofd;
	    numfiles = numfiles + 1;
	    if (verbose_flag ==1)
	      {
		printf("--wronly ");
		printf("%2\n", optarg);
	      }
	    break;
	  }
	case 'm':
	  {
	    int start;
	    start = optind;
     	    char* s;
	    int index = 0;
	    char** params = malloc(argc*sizeof(char*));
	    while (optind < argc)
	      {
		s = argv[optind];
		if (strlen(s) > 1 && s[0] == '-' && s[1] == '-')
		  {
		    break;
		  }
		//cannot run multiple commands if always restart params at index 0 so should make index a global variable and increment
		params[index] = s;
		index = index + 1;
		optind = optind + 1;
	      }
	    params[index] = NULL;
	    start = 1;
	    if (verbose_flag == 1)
	      {
		char* printout = "--command ";
		write(1, "--command ", strlen("--command "));
		while (start < index && strcmp(params[start], ">") != 0 && strcmp(params[start], "<") != 0)
		  {
		    int length = strlen(params[start]);
		    write (1, params[start], length);
		    write(1, " ", 1);
		    start = start + 1;
		  }
		write(1, "\n", 1);
	      }
	    int in = atoi(params[0]);
	    int out = atoi(params[1]);
	    int err = atoi(params[2]);
   	    dup2(fileds[in], 0);
	    dup2(fileds[out], 1);
	    dup2(fileds[err], 2);
	    //execvp(params[3], &params[3]);
	    execute(params);
	    
	   
	    break;
	  }
	}
    }
    if (writefile)
      {
	int ofd = open(writefile, O_WRONLY);
	fileds[numfiles] = ofd;
      }
}
