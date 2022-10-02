#include "calcpi.h"
#include <pthread.h>
#include <thread>
#include <vector>
#include <stdlib.h>
#include <tgmath.h>

uint64_t counts = 0;

struct TMem {
  pthread_t tid;
  int threads;
  unsigned long long r1, results, rsq1, i1;
};

void * doLoop(void * targ) {
  struct TMem * tm = (struct TMem *) targ;
  uint64_t count = 0;
  for (unsigned long long x = tm->i1; x <= tm->r1; x += tm->threads) {
    for (unsigned long long y = 0; y <= tm->r1; y ++) {
      if( x*x + y*y <= tm->rsq1) count++;
       
    }
  }
  tm->results = count;
  //printf("%lld\n", tm->results);
  //count += tm->results;
  pthread_exit(0);
}
uint64_t count_pixels(int r, int n_threads)
{
  TMem tarr[n_threads];
  double rsq = double(r) * r;
  

  for (int i = 0; i < n_threads; i++) {
    tarr[i].r1 = r; tarr[i].rsq1 = rsq; tarr[i].threads = n_threads; tarr[i].i1 = i+1;
    if (0 != pthread_create(&tarr[i].tid, 0, doLoop, &tarr[i])) {
      printf("Error: pthread_create failed\n"); exit(-1);
    }
  }

  for (int i = 0; i < n_threads; i++) {
    pthread_join(tarr[i].tid, 0);
    counts += tarr[i].results;
  }

  return counts * 4 + 1;
}


