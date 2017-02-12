#include <time.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>

long long iterationsnum = 0;
int opt_yield = 0;
pthread_mutex_t lock;
char sc = ' ';
void add(long long *pointer, long long value) {
  long long sum = *pointer + value;
  if (opt_yield)
    {
      sched_yield();
    }
  *pointer = sum;
}

void addcompare(long long *pointer, long long value){
  long long previous;
  long long sum;
  do {
    previous = *pointer;
    sum = previous + value;
    if (opt_yield)
      {
	sched_yield();
      }
  } while(__sync_val_compare_and_swap(pointer, previous, sum) != previous);
}

void * threadadd(void *counter)
{
  long ct;
  int synclock = 0;
  for (ct = 0; ct < iterationsnum; ct++)
    {
      if (sc == 'm')
	{
	  pthread_mutex_lock(&lock);
	  add((long long*)counter, 1);
	  pthread_mutex_unlock(&lock);
	}
      else if (sc == 's')
	{
	  while (__sync_lock_test_and_set(&synclock, 1)) while (synclock);
	  add((long long*)counter, 1);
	  __sync_lock_release(&synclock);
	}
      else if (sc == 'c')
	{
	  addcompare((long long*)counter, 1);
	}
      else
	{
	  add((long long*)counter, 1);
	}
    }
    for (ct = 0; ct < iterationsnum; ct++)
    {
      if (sc == 'm')
	{
	  pthread_mutex_lock(&lock);
	  add((long long*)counter, -1);
	  pthread_mutex_unlock(&lock);
	}
      else if (sc == 's')
	{
	  while(__sync_lock_test_and_set(&synclock, 1)) while (synclock);
	  add((long long *)counter, -1);
	  __sync_lock_release(&synclock);
	}
      else if (sc == 'c')
	{
	  addcompare((long long*)counter, -1);
	}
      else
	{
	  add((long long*)counter, -1);
	}
    }

}

int main (int argc, char** argv)
{
  int c = 0;
  long threadnum = 1;
  long long count = 0;
  struct option longopts[] =
    {
      {"threads",optional_argument,0,'t'},
      {"iterations",optional_argument,0,'i'},
      {"yield", no_argument, &opt_yield, 1},
      {"sync", required_argument, 0, 's'},
      {0, 0, 0, 0}
    };
  //  long long count = 0;
  while((c=getopt_long(argc,argv,":t:i:s", longopts, 0)) != -1)
    {
      switch (c)
	{
	case 't':
	  threadnum = atoi(optarg);
	  break;
	case 'i':
	  iterationsnum = atoi(optarg);
	  break;
	case 's':
	  if(strlen(optarg) == 1)
	    if(optarg[0] == 'm')
	      sc = 'm';
	    else if (optarg[0] == 's')
	      sc = 's';
	    else if (optarg[0] == 'c')
	      sc = 'c';
	    else
	      {
		printf("Error: wrong argument length");
		return 1;
	      }
	  break;
	}
    }

  if (pthread_mutex_init(&lock, NULL) != 0)
    {
      printf("ERROR: mutex init failed");
      return 1;
    }
  
  struct timespec start;
  clock_gettime(CLOCK_MONOTONIC, &start);
  pthread_t threads[threadnum];
  pthread_attr_t attr;
  void *status;
  int rc;
  long t;
  if (sc == 'm')
    {
      pthread_mutex_init(&lock, NULL);
    }
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
  for (t = 0; t<threadnum; t++)
    {
      rc = pthread_create(&threads[t], &attr, threadadd, &count);
      if (rc)
	{
	  printf("ERROR: pthread_create() didn't work");
	  exit(-1);
	}
    }
  pthread_attr_destroy(&attr);
  for (t = 0; t<threadnum;t++)
    {
      rc = pthread_join(threads[t], &status);
      if (rc)
	{
	  printf("ERROR: bad return from pthread_join");
	  exit(-1);
	}
    }
  struct timespec end;
  clock_gettime(CLOCK_MONOTONIC, &end);
  long long time = ((long long)1000000000*(end.tv_sec-start.tv_sec) + (long long)(end.tv_nsec-start.tv_nsec));
  long operations = threadnum*iterationsnum*2;
 
  char* str;
  
  if (opt_yield)
    {
      if (sc == 'm')
	str = "add-yield-m";
      else if (sc == 's')
	str = "add-yield-s";
      else if (sc == 'c')
	str = "add-yield-c";
      else
	str = "add-yield";
    }
  else
    if (sc == 'm')
      str = "add-m";
    else if (sc == 's')
      str = "add-s";
    else if (sc == 'c')
      str = "add-c";
    else
      str = "add-none";
  {
    printf("%s,%ld,%ld,%ld,%lld,%ld,%lld\n",str, threadnum, iterationsnum, operations, time, time/operations, count); 
    }
  pthread_mutex_destroy(&lock);
}
