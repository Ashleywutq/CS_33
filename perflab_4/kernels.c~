/********************************************************
 * Kernels to be optimized for the CS:APP Performance Lab
 ********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "defs.h"
#include <pthread.h>
#include <semaphore.h>
#include <algorithm>
#include <string.h>
#include "helper.h"

pthread_t threads[32];

/* 
 * Please fill in the following team struct 
 */
user_t user = {
    (char*)  "1232131",            /* UID */
    (char*)  "Your Name",          /* Full name */
    (char*)  "yourname@ucla.edu",  /* Email */
};

//If you worked with anyone in this project, please indicate that here:
// I worked with  "... , ... ".

// Of course, your entire file should be hand-written by you.  You are free to
// look at tutorials and academic literature for radix-sort based sorting.  You
// are not allowed to look at or copy from implementations on online code repositories
// like github.


//  You should modify only this file, but feel free to modify however you like!


/*
 * setup - This function runs one time, and will not be timed.
 *         You can do whatever initialization you need here, but
 *         it is not required -- don't use if you don't want to.  ^_^
 */
void setup() {
  //..

  // So, in my experiments, it take fewer cycles for each run if we 
  // waste some time doing absolute nothing in particular
  // at the begining of the program.  It might be because of Intel's Turbo
  // mode and DVFS somehow??  TBH IDK, but I would leave this in if I were
  // you.
  for(int i = 0; i < 50000;++i) {
     do_nothing(i);
  }
}

/******************************************************
 * Your different versions of the singlethread kernel go here
 ******************************************************/
bool kvp_compare(kvp lhs, kvp rhs) { 
  return lhs.key < rhs.key; 
}

/*
 * naive_singlethread - The naive baseline version of singlethread 
 */
char naive_singlethread_descr[] = "naive_singlethread: Naive baseline implementation";
void naive_singlethread(int dim, kvp *src, kvp *dst) 
{
    //This is the built-in stable sort if you want to try it
    //memcpy(dst, src, dim*sizeof(kvp));
    //std::stable_sort(dst, dst+dim, kvp_compare);
    //return;

    int log_radix=8; //Radix of radix-sort is 2^8
    int iters=(sizeof(unsigned int)*8/log_radix);

    // 256 buckets for 2^8 bins, one for each iteration 
    unsigned long long buckets[256+1][iters];
    unsigned long long sum[256+1][iters];

    for(int iter = 0; iter < iters; ++iter) {
      for(int i = 0; i < bucket_size(log_radix); ++i) {
        buckets[i][iter]=0;
        sum[i][iter]=0;
      }

      //1. Generate the bucket count
      for(int i = 0; i < dim; ++i) {
        int index = gen_shift(src[i].key,iter*log_radix,
                              (bucket_size(log_radix)-1))+1;
        buckets[index][iter]++;
      }

      //2. Perform scan
      for(int i = 1; i < bucket_size(log_radix); ++i) {
        sum[i][iter] = buckets[i][iter] + sum[i-1][iter];
      }

      //3. Move Data items
      for(int i = 0; i < dim; ++i) {
        int index = gen_shift(src[i].key,iter*log_radix,
                              bucket_size(log_radix)-1);
        int out_index = sum[index][iter];
        move_kvp(dst,src,i,out_index);
        sum[index][iter]++;
      }

      // Move dest back to source
      for(int i = 0; i < dim; ++i) {
        move_kvp(src,dst,i,i);
      }

    }
}


/*
 * singlethread - Your current working version of singlethread. 
 * IMPORTANT: This is the version you will be graded on
 */
char singlethread_descr[] = "singlethread: Current working version";
void singlethread(int dim, kvp *src, kvp *dst) 
{
  return naive_singlethread(dim,src,dst);
}


/********************************************************************* 
 * register_singlethread_functions - Register all of your different versions
 *     of the singlethread kernel with the driver by calling the
 *     add_singlethread_function() for each test function.  When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_singlethread_functions() {
    add_singlethread_function(&naive_singlethread, naive_singlethread_descr);
    add_singlethread_function(&singlethread, singlethread_descr);
    /* ... Register additional test functions here */
}



// ----------------------- do multi-threaded versions here ------------------


// I'm kind of cheating to pass global variables to my thread function
// There are nicer ways to do this, but w/e
int gdim;
kvp* gsrc;
kvp* gdst;

void *do_sort(void* threadid) {
   long tid = (long)threadid;
   //printf("Hello from thread %d\n", tid);
   int dim = gdim;
   kvp* src = gsrc;
   kvp* dst = gdst;
  
   naive_singlethread(dim,src,dst);   
   return 0;
}


/* 
 * naive_multithread - The naive baseline version of multithread 
 */
char naive_multithread_descr[] = "naive_multithread: Naive baseline implementation";
void naive_multithread(int dim, kvp *src, kvp *dst) 
{
    gdim=dim;
    gsrc=src;
    gdst=dst;

    //call one thread to do our work -- I'm sure that will make things go faster
    pthread_create(&threads[0], NULL, do_sort, (void *)0 /*tid*/);

    void** ret=0;
    pthread_join(threads[0],ret);
}


/* 
 * multithread - Your current working version of multithread
 * IMPORTANT: This is the version you will be graded on
 */
char multithread_descr[] = "multithread: Current working version";
void multithread(int dim, kvp *src, kvp *dst) 
{
    naive_multithread(dim,src,dst);
}

/*********************************************************************
 * register_multithread_functions - Register all of your different versions
 *     of the multithread kernel with the driver by calling the
 *     add_multithread_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_multithread_functions() 
{
    add_multithread_function(&naive_multithread, naive_multithread_descr);   
    add_multithread_function(&multithread, multithread_descr);   
    /* ... Register additional test functions here */
}

