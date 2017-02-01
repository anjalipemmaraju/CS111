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
#include <sys/wait.h>
//check verbose with many flags

void signal_handler(int signum)
{
  fprintf(stderr, "%d caught", signum);
  exit(signum);
}



int main (int argc, char ** argv)
{

  int c = 0;
  int verbose_flag = 0;
  int append_flag = 0;
  int cloexec_flag = 0;
  int creat_flag = 0;
  int directory_flag = 0;
  int dsync_flag = 0;
  int excl_flag = 0;
  int nofollow_flag = 0;
  int nonblock_flag = 0;
  int rsync_flag = 0;
  int sync_flag = 0;
  int trunc_flag = 0;
  pid_t pid[argc];				       
  int wait_flag = 0;
  char*** buff = malloc(sizeof(char**)); // what size should i make this
  struct option longopts[] =
    {
      {"rdonly", required_argument, 0, 'r' },
      {"wronly", required_argument, 0,'w'},
      {"verbose", no_argument, &verbose_flag, 1},
      {"command", no_argument, 0, 'm'},
      {"append", no_argument, &append_flag, 1},
      {"cloexec", no_argument, &cloexec_flag, 1},
      {"creat", no_argument, &creat_flag, 1},
      {"directory", no_argument, &directory_flag, 1},
      {"dsync", no_argument, &dsync_flag, 1},
      {"excl", no_argument, &excl_flag, 1},
      {"nofollow", no_argument, &nofollow_flag, 1},
      {"nonblock", no_argument, &nonblock_flag, 1},
      {"rsync", no_argument, &rsync_flag, 1},
      {"sync", no_argument, &sync_flag, 1},
      {"trunc", no_argument, &trunc_flag, 1},
      {"rdwr", required_argument, 0, 'f'},
      {"catch", required_argument, 0, 'z'},
      {"abort", no_argument, 0, 'a'},
      {"ignore", required_argument, 0, 'i'},
      {"default", required_argument, 0, 'd'},
      {"pause", no_argument, 0, 'u'},
      {"pipe", no_argument, 0, 'p'},
      {"close", required_argument, 0, 'q'},
      {"wait", no_argument, &wait_flag, 1},
      {0, 0, 0, 0}    
    };

  int ief = 0;
  for (ief ; ief < argc; ief++)
    {
      pid[ief] = 0;
    }
  ief = 0;
  char* inputfile = NULL;
  char* writefile = NULL;
  char* rwfile = NULL;
  int fileds[2*argc];
  int numfiles = 0;
  int tem[2];
  while ((c = getopt_long (argc, argv, "p:ud:i:az:r:w:f:m", longopts, 0)) != -1)
    {
      
      switch (c)
	{
	case 'q':
	  {
	    int index = atoi(optarg);
	    int fd = fileds[index];
	    close(fd);
	    break;
	  }
	case 'p':
	  {
	    int rr = pipe(tem);
	    if (rr == -1)
	      {
		fprintf(stderr, "error making pipe");
	      }
	    fileds[numfiles] = tem[0];
	    numfiles++;
	    fileds[numfiles] = tem[1];
	    numfiles++;
	    if (verbose_flag)
	      {
		printf("--pipe");
	      }
	    break;
	  }
	case 'u':
	  {
	    if (verbose_flag)
	      {
		printf("--pause");
	      }
	    pause();
	    break;
	  }
	case 'd':
	  {
	    if (verbose_flag)
	      {
		printf("--default ");
		printf("%s", optarg);
	      }
	    int ar = atoi(optarg);
	    signal(ar, SIG_DFL);
	    break;
	  }
	case 'i':
	  {
	    if (verbose_flag)
	      {
		printf("--ignore ");
		printf("%s", optarg);
	      }
	    int ar = atoi(optarg);
	    signal(ar, SIG_IGN);
	    break;
	  }
	case 'a':
	  {
	    char* nu  = NULL;
	    int a = *nu;
	    break;
	  }
	case 'z':
	  {
	    int ar = atoi(optarg);
	    signal(ar, signal_handler);
	    break;
	  }
	case 'r':
	  {
	    inputfile = optarg;
	    int ifd = open(inputfile, O_RDONLY|append_flag*O_APPEND|cloexec_flag*O_CLOEXEC|creat_flag*O_CREAT|directory_flag*O_DIRECTORY|dsync_flag*O_DSYNC|excl_flag*O_EXCL|nofollow_flag*O_NOFOLLOW|nonblock_flag*O_NOFOLLOW|rsync_flag*O_RSYNC|sync_flag*O_SYNC|trunc_flag*O_TRUNC, 0644);
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
	case 'w':
	  {
	    writefile = optarg;
	    int ofd = open(writefile, O_WRONLY|append_flag*O_APPEND|cloexec_flag*O_CLOEXEC|creat_flag*O_CREAT|directory_flag*O_DIRECTORY|dsync_flag*O_DSYNC|excl_flag*O_EXCL|nofollow_flag*O_NOFOLLOW|nonblock_flag*O_NOFOLLOW|rsync_flag*O_RSYNC|sync_flag*O_SYNC|trunc_flag*O_TRUNC, 0644);
	    // int ofd = open(writefile, O_WRONLY);
	    fileds[numfiles] = ofd;
	    numfiles = numfiles + 1;
	    if (verbose_flag ==1)
	      {
		printf("--wronly ");
		printf("%s\n", optarg);
	      }
	    break;
	  }
	case 'f':
	  {
	    {
	    rwfile = optarg;
	    int ofd = open(rwfile, O_RDWR|append_flag*O_APPEND|cloexec_flag*O_CLOEXEC|creat_flag*O_CREAT|directory_flag*O_DIRECTORY|dsync_flag*O_DSYNC|excl_flag*O_EXCL|nofollow_flag*O_NOFOLLOW|nonblock_flag*O_NOFOLLOW|rsync_flag*O_RSYNC|sync_flag*O_SYNC|trunc_flag*O_TRUNC);
	    // int ofd = open(writefile, O_WRONLY);
	    fileds[numfiles] = ofd;
	    numfiles = numfiles + 1;
	    if (verbose_flag ==1)
	      {
		printf("--rdwr ");
		printf("%s\n", optarg);
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
	    //execvp(params[3], &params[3]);
	    //	    execute(params);
	    int    status = 0;


	    buff[ief] = &params[3];
	    pid[ief] = fork();
	    
	    
	    	    
	    int aa = 0;
	    if (pid[ief] < 0) {    
	      printf("*** ERROR: forking child process failed\n");
	      exit(1);

	      //close all files HERE
	    }
	    else if (pid[ief] == 0) {

	      if (dup2(fileds[in], 0) == -1)
		fprintf(stderr, "call dup2fail 1");
	      if (dup2(fileds[out], 1) == -1)
		fprintf(stderr, "call dup2fail 2");
	      
	      if (dup2(fileds[err], 2) == -1)
		fprintf(stderr, "call dup2fail 3");
	      
	      
	      for (aa; aa < numfiles; aa++)
		{
		  close(fileds[aa]);
		}
	      if (execvp(params[3], &params[3]) < 0) { 
		printf("*** ERROR: exec failed\n");
		exit(1);
	      }
	    }
	    else
	      {
		ief++;
	      }
	    
	    break;
	  }
	  }
	}
    
    }
  
  if(wait_flag == 1)
    {
      int done[ief];
      int qq;
      int check = 0;
      int status;
      for (qq = 0; qq < ief; qq++)
	{
	  done[qq] = 0;
	}
      while (check != ief)
	{
	  for (qq = 0; qq < ief; qq++)
	    {
	      if (done[qq] == 0)
		if (waitpid(pid[qq], &status, WNOHANG)!= 0)
		  {
		    done[qq] = 1;
		    check++;
		    char** toprint = buff[qq];
		    int j;
		    for (j = 0; buff[qq][j] != NULL; j++)
		      {
			printf("%d ", WEXITSTATUS(status));
			printf("%s ", buff[qq][j]);
		      }
		    fprintf(stdout, "\n");
		  } 
	    }
	}
    }
}
