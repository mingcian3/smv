#define INTHREADER
#include "options.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "MALLOCC.h"
#include "threader.h"

#ifdef pp_SAMPLE

/* ------------------ Sample ------------------------ */

void *Sample(void *arg){
 // n_sample_threads - number of threads - default 1
 // use_sample_threads 0/1 use multi threading or not
 // threads - data structure that holds threading instance
 // Sample - routine that does the work, must end with THREAD_EXIT macro

  sample code

  THREAD_EXIT(sample_threads);
}
//*** call before first use of threading routines

sample_threads = THREADinit(&n_sample_threads, &use_sample_threads, Sample);

//*** call to do the work
THREADrun(sample_threads, arg);
#endif

/* ------------------ THREADinit ------------------------ */

threaderdata *THREADinit(int *nthreads_ptr, int *use_threads_ptr, void *(*run_arg)(void *arg)){
  threaderdata *thi;
  int nthreads_local=1, use_threads_local=0;

  //create a routine
    // void *run(void *arg){
    //   run();
    //   pthread_exit(NULL); THREAD_EXIT macros handles these two lines
    //   return NULL;
    // }

  NewMemory(( void ** )&thi, sizeof(threaderdata));

  if(nthreads_ptr != NULL && *nthreads_ptr > 1)nthreads_local = *nthreads_ptr;
  if(nthreads_local > MAX_THREADS)nthreads_local = MAX_THREADS;
  if(use_threads_ptr != NULL && *use_threads_ptr != 0)use_threads_local = 1;

  thi->count = 0;
  thi->n_threads_ptr   = nthreads_ptr;
  thi->use_threads_ptr = use_threads_ptr;
  thi->n_threads       = nthreads_local;
  thi->use_threads     = use_threads_local;
  thi->run             = run_arg;
#ifdef pp_THREAD
  NewMemory(( void ** )&thi->thread_ids, nthreads_local * sizeof(pthread_t));
  pthread_mutex_init(&thi->mutex, NULL);
#endif
  return thi;
}

/* ------------------ THREADcontrol ------------------------ */

void THREADcontrol(threaderdata *thi, int var){
#ifdef pp_THREAD
  if(thi == NULL)return;
  switch(var){
  case THREAD_LOCK:
    if(thi->use_threads == 1)pthread_mutex_lock(&thi->mutex);
    break;
  case THREAD_UNLOCK:
    if(thi->use_threads == 1)pthread_mutex_unlock(&thi->mutex);
    break;
  case THREAD_JOIN:
    if(thi->use_threads == 1&&thi->count>0){
      int i;

      for(i = 0;i < thi->n_threads;i++){
        pthread_join(thi->thread_ids[i], NULL);
      }
    }
    break;
  default:
    assert(0);
    break;
  }
#endif
}

/* ------------------ THREADrun ------------------------ */

void THREADrun(threaderdata *thi, void *arg){
#ifdef pp_THREAD
  if(thi == NULL)return;
  if(thi->use_threads_ptr!=NULL)thi->use_threads = *(thi->use_threads_ptr);
  if(thi->n_threads_ptr != NULL){
    thi->n_threads = *(thi->n_threads_ptr);
    if(thi->n_threads>MAX_THREADS)thi->n_threads = MAX_THREADS;
  }
  if(thi->use_threads == 1){
    int i;

    thi->count = thi->n_threads;
    for(i = 0; i < thi->n_threads; i++){
      pthread_create(thi->thread_ids + i, NULL, thi->run, arg);
    }
  }
  else{
    thi->count = 1;
    thi->run(arg);
  }
#else
  thi->run(arg);
#endif
}
