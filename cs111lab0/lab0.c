#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>

void signal_handler(int signum)
{
  if (signum == SIGSEGV)
    {
      perror("Segmentation Fault caught");
      exit(3);
    }

}

void copy (int input, int output)
{
  char* buff = (char*)malloc(sizeof(char));
  int status = read(input, buff, 1);
  while (status > 0)
    {
      write(output, buff, 1);
      status = read(input, buff, 1);
    }

}

int main (int argc, char ** argv)
{

  int c = 0;
  struct option longopts[] =
    {
      {"input", required_argument, 0, 'i' },
      {"output", required_argument, 0,'o'},
      {"segfault", no_argument, 0, 's'},
      {"catch", no_argument, 0, 'c'}
    
    };
  char* inputfile = NULL;
  char* outputfile = NULL;
  int segflag = 0;
  char* fault = NULL;
  while ((c = getopt_long (argc, argv, "i:o:sc", longopts, NULL)) != -1)
    {
      switch (c)
	{
	case 'i':
	  inputfile = optarg;
	  break;
	case 'o':
	  outputfile = optarg;
	  break;
	case 's':
	  segflag = 1;
	  break;
	case 'c':
	  signal(SIGSEGV, signal_handler);
	  break;
	}
    }
    if (segflag == 1)
      {
	char forcefault = *fault;
      }
    if (inputfile)
      {
	int ifd = open(inputfile, O_RDONLY);
	if (ifd >= 0)
	  {
	    close(0);
	    dup(ifd);
	    close(ifd);
	  }
	else
	  {
	    fprintf(stderr, "error opening input file");
	    perror("Error");
	    exit(1);
	  }
      }
    if (outputfile)
      {
	int ofd = creat(outputfile, 0666);
	if (ofd >= 0)
	  {
	    close(1);
	    dup(ofd);
	    close(ofd);
	  }
	else
	  {
	    fprintf(stderr,"error opening output file");
	    perror("Error");
	    exit(2);
	  }
      }

    copy(0, 1);
    exit(0);
}
