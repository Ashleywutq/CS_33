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

/* 
 * Please fill in the following team struct 
 */
user_t user = {
    (char*)  "204612415",            /* UID */
    (char*)  "Ashley Wu",          /* Full name */
    (char*)  "ashley-wu@live.com",  /* Email */
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
      for(int i = 0; i < bucket_size(log_radix)+1; ++i) {
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
    int log_radix=8; //Radix of radix-sort is 2^8
    int iters=4; //(sizeof(unsigned int)*8/log_radix);
    int bucketsize=256;
    
    // 256 buckets for 2^8 bins, one for each iteration
    unsigned long long buckets[iters][256+1];
    unsigned long long sum[iters][256+1];
    
    //zero initialize
    memset(buckets,0,sizeof(buckets));
    memset(sum,0,sizeof(sum));;
    
    //1. Generate the bucket count
    for(int i = 0; i < dim; ++i) {
        int key = src[i].key;
        
        int index = (key&(bucketsize-1))+1;
        buckets[0][index]++;
        
        int index1 = ((key>>(1*log_radix))&
                      (bucketsize-1))+1;
        buckets[1][index1]++;
        
        int index2 = ((key>>(2*log_radix))&
                      (bucketsize-1))+1;
        buckets[2][index2]++;
        
        int index3 = ((key>>(3*log_radix))&
                      (bucketsize-1))+1;
        buckets[3][index3]++;
        
    }
    
    //2. Perform scan
    for(int i = 1; i < bucketsize; ++i) {
        sum[0][i] = buckets[0][i] + sum[0][i-1];
        sum[1][i] = buckets[1][i] + sum[1][i-1];
        sum[2][i] = buckets[2][i] + sum[2][i-1];
        sum[3][i] = buckets[3][i] + sum[3][i-1];
    }
    
    for(int iter = 0; iter < iters; iter++) {
        
        
        //3. Move Data items
        for(int i = 0; i < dim; i+=8) {
            int index = ((src[i].key>>(iter*log_radix))&
                         (bucketsize-1));
            int out_index = sum[iter][index];
            dst[out_index]=src[i];
            sum[iter][index]++;
            
            int index1 = ((src[i+1].key>>(iter*log_radix))&
                          (bucketsize-1));
            int out_index1 = sum[iter][index1];
            dst[out_index1]=src[i+1];
            sum[iter][index1]++;
            
            int index2 = ((src[i+2].key>>(iter*log_radix))&
                          (bucketsize-1));
            int out_index2 = sum[iter][index2];
            dst[out_index2]=src[i+2];
            sum[iter][index2]++;
            
            int index3 = ((src[i+3].key>>(iter*log_radix))&
                          (bucketsize-1));
            int out_index3 = sum[iter][index3];
            dst[out_index3]=src[i+3];
            sum[iter][index3]++;
            
            int index4 = ((src[i+4].key>>(iter*log_radix))&
                          (bucketsize-1));
            int out_index4 = sum[iter][index4];
            dst[out_index4]=src[i+4];
            sum[iter][index4]++;
            
            int index5 = ((src[i+5].key>>(iter*log_radix))&
                          (bucketsize-1));
            int out_index5 = sum[iter][index5];
            dst[out_index5]=src[i+5];
            sum[iter][index5]++;
            
            int index6 = ((src[i+6].key>>(iter*log_radix))&
                          (bucketsize-1));
            int out_index6 = sum[iter][index6];
            dst[out_index6]=src[i+6];
            sum[iter][index6]++;
            
            int index7 = ((src[i+7].key>>(iter*log_radix))&
                          (bucketsize-1));
            int out_index7 = sum[iter][index7];
            dst[out_index7]=src[i+7];
            sum[iter][index7]++;
        }
        
        //second half of loop
        iter++;
        
        //3. Move Data items
        for(int i = 0; i < dim; i+=8) {
            int index = ((dst[i].key>>(iter*log_radix))&
                         (bucketsize-1));
            int out_index = sum[iter][index];
            src[out_index]=dst[i];
            sum[iter][index]++;
            
            int index1 = ((dst[i+1].key>>(iter*log_radix))&
                          (bucketsize-1));
            int out_index1 = sum[iter][index1];
            src[out_index1]=dst[i+1];
            sum[iter][index1]++;
            
            int index2 = ((dst[i+2].key>>(iter*log_radix))&
                          (bucketsize-1));
            int out_index2 = sum[iter][index2];
            src[out_index2]=dst[i+2];
            sum[iter][index2]++;
            
            int index3 = ((dst[i+3].key>>(iter*log_radix))&
                          (bucketsize-1));
            int out_index3 = sum[iter][index3];
            src[out_index3]=dst[i+3];
            sum[iter][index3]++;
            
            int index4 = ((dst[i+4].key>>(iter*log_radix))&
                          (bucketsize-1));
            int out_index4 = sum[iter][index4];
            src[out_index4]=dst[i+4];
            sum[iter][index4]++;
            
            int index5 = ((dst[i+5].key>>(iter*log_radix))&
                          (bucketsize-1));
            int out_index5 = sum[iter][index5];
            src[out_index5]=dst[i+5];
            sum[iter][index5]++;
            
            int index6 = ((dst[i+6].key>>(iter*log_radix))&
                          (bucketsize-1));
            int out_index6 = sum[iter][index6];
            src[out_index6]=dst[i+6];
            sum[iter][index6]++;
            
            int index7 = ((dst[i+7].key>>(iter*log_radix))&
                          (bucketsize-1));
            int out_index7 = sum[iter][index7];
            src[out_index7]=dst[i+7];
            sum[iter][index7]++;
        }
    }
    
    // Move dest back to source
    for(int i = 0; i < dim; i+=8) {
        dst[i] = src[i];
        dst[i+1] = src[i+1];
        dst[i+2] = src[i+2];
        dst[i+3] = src[i+3];
        dst[i+4] = src[i+4];
        dst[i+5] = src[i+5];
        dst[i+6] = src[i+6];
        dst[i+7] = src[i+7];
    }

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
pthread_t threads[32];

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
pthread_t thread_id[32];


int ndim;
int ntn;
kvp* nsrc;
kvp* ndst;

// 256 buckets for 2^8 bins, one for each iteration
unsigned long long buckets[4][17][256+1];

int niter;

//barriers for threads
pthread_barrier_t barrier1;
pthread_barrier_t barrier2;
pthread_barrier_t barrier3;
pthread_barrier_t barrier4;
pthread_barrier_t barrier5;
pthread_barrier_t barrier6;
pthread_barrier_t barrier7;
pthread_barrier_t barrier8;
pthread_barrier_t barrier9;
pthread_barrier_t barrier10;



void* thread_routine(void* arg){
    
    long tid = (long)arg;
    int iter = niter;
    
    //printf("Hello from thread %d\n", tid);
    int dim = ndim;
    kvp* src = nsrc;
    kvp* dst = ndst;
    
    int dimoffset = dim/ntn*tid;
    int sumoffset = 256/ntn*tid;
    int bucketsize=256;
    int log_radix=8; //Radix of radix-sort is 2^8
    
    
    //initialize buckets
    for(int i = 0; i < 257; i++) {
        buckets[iter][tid][i]=0;
    }
    
    //initialize bucket sums
    for (int i = 0+sumoffset;i<256/ntn+sumoffset;i++){
        buckets[iter][ntn][i]=0;
    }
    
    
    //wait for other threads
    pthread_barrier_wait(&barrier1);
    
    //1. Generate the bucket count
    for(int i = 0+dimoffset; i< dim/ntn+dimoffset; ++i) {
        int index = ((src[i].key>>(iter*log_radix))&
                     (bucketsize-1))+1;
        buckets[iter][tid][index]++;
    }
    
    //wait for other threads
    pthread_barrier_wait(&barrier2);
    
    //calculate total bucket 1-256
    for (int i = 0+sumoffset;i<256/ntn+sumoffset;i++){
        int temp[ntn];
        for (int j=0;j<ntn;j++){
            temp[j]=buckets[iter][j][i];
        }
        buckets[iter][0][i]=0;
        for (int j=1;j<ntn+1;j++){
            buckets[iter][j][i]= buckets[iter][j-1][i]+temp[j-1];
        }
    }
    
    if (tid == ntn-1){
        int i=256;
        int temp[ntn-1];
        for (int j=0;j<ntn-1;j++){
            temp[j]=buckets[iter][j][i];
        }
        buckets[iter][0][i]=0;
        for (int j=1;j<ntn;j++){
            buckets[iter][j][i]= buckets[iter][j-1][i]+temp[j-1];
        }
    }
    
    //wait for other threads
    pthread_barrier_wait(&barrier3);
    
    ////2. Perform scan: calculate sum
    if (tid == 0){
        int old = 0;
        for(int i = 1; i < bucketsize; ++i) {
            int newnum = buckets[iter][ntn][i];
            buckets[iter][ntn][i]=old+newnum;
            old =old+newnum;
        }
    }
    
    //wait for other threads
    pthread_barrier_wait(&barrier4);
    
    //3. Move Data items: calculate where to move
    
    for (int i = 1;i<257;i++){
        buckets[iter][tid][i]+=buckets[iter][ntn][i-1];
    }
    
    //move data
    for(int i = 0+dimoffset; i< dim/ntn+dimoffset; ++i) {
        int index = ((src[i].key>>(iter*log_radix))&
                     (bucketsize-1))+1;
        int out_index = buckets[iter][tid][index];
        dst[out_index]=src[i];
        buckets[iter][tid][index]++;
    }
    
    //wait for other threads
    pthread_barrier_wait(&barrier5);
    
    /////////////////////////////////////////////////////////////////////////////////
    iter++;
    src = ndst;
    dst = nsrc;
    
    //initialize buckets
    for(int i = 0; i < 257; i++) {
        buckets[iter][tid][i]=0;
    }
    
    //initialize bucket sums
    for (int i = 0+sumoffset;i<256/ntn+sumoffset;i++){
        buckets[iter][ntn][i]=0;
    }
    
    
    //wait for other threads
    pthread_barrier_wait(&barrier6);
    
    
    //1. Generate the bucket count
    for(int i = 0+dimoffset; i< dim/ntn+dimoffset; ++i) {
        int index = ((src[i].key>>(iter*log_radix))&
                     (bucketsize-1))+1;
        buckets[iter][tid][index]++;
    }
    
    //wait for other threads
    pthread_barrier_wait(&barrier7);
    
    //calculate total bucket 1-256
    for (int i = 0+sumoffset;i<256/ntn+sumoffset;i++){
        int temp[ntn];
        for (int j=0;j<ntn;j++){
            temp[j]=buckets[iter][j][i];
        }
        buckets[iter][0][i]=0;
        for (int j=1;j<ntn+1;j++){
            buckets[iter][j][i]= buckets[iter][j-1][i]+temp[j-1];
        }
    }
    
    if (tid == ntn-1){
        int i=256;
        int temp[ntn-1];
        for (int j=0;j<ntn-1;j++){
            temp[j]=buckets[iter][j][i];
        }
        buckets[iter][0][i]=0;
        for (int j=1;j<ntn;j++){
            buckets[iter][j][i]= buckets[iter][j-1][i]+temp[j-1];
        }
    }
    
    //wait for other threads
    pthread_barrier_wait(&barrier8);
    
    ////2. Perform scan: calculate sum
    if (tid == 0){
        int old = 0;
        for(int i = 1; i < bucketsize; ++i) {
            int newnum = buckets[iter][ntn][i];
            buckets[iter][ntn][i]=old+newnum;
            old =old+newnum;
        }
    }
    
    //wait for other threads
    pthread_barrier_wait(&barrier9);
    
    //3. Move Data items: calculate where to move
    
    for (int i = 1;i<257;i++){
        buckets[iter][tid][i]+=buckets[iter][ntn][i-1];
    }
    
    //move data
    for(int i = 0+dimoffset; i< dim/ntn+dimoffset; ++i) {
        int index = ((src[i].key>>(iter*log_radix))&
                     (bucketsize-1))+1;
        int out_index = buckets[iter][tid][index];
        dst[out_index]=src[i];
        buckets[iter][tid][index]++;
    }
    
    //wait for other threads
    pthread_barrier_wait(&barrier10);
    
    // Move dest back to source
    if (iter == 3){
        for(int i = 0+dimoffset; i< dim/ntn+dimoffset; ++i) {
            src[i] = dst[i];
        }
        
    }
    
    return NULL;
}


char multithread_descr[] = "multithread: Current working version";
void multithread(int dim, kvp *src, kvp *dst)
{
    ndim=dim;
    nsrc=src;
    ndst=dst;
    
    if (dim < 65536){
        return singlethread(dim, src, dst);
    }
    if (dim <100000){
        ntn = 4;
        
        int iters=4; //(sizeof(unsigned int)*8/log_radix);
        
        for(niter = 0; niter < iters; niter+=2) {
            
            pthread_barrier_init(&barrier1,NULL,ntn);
            pthread_barrier_init(&barrier2,NULL,ntn);
            pthread_barrier_init(&barrier3,NULL,ntn);
            pthread_barrier_init(&barrier4,NULL,ntn);
            pthread_barrier_init(&barrier5,NULL,ntn);
            pthread_barrier_init(&barrier6,NULL,ntn);
            pthread_barrier_init(&barrier7,NULL,ntn);
            pthread_barrier_init(&barrier8,NULL,ntn);
            pthread_barrier_init(&barrier9,NULL,ntn);
            pthread_barrier_init(&barrier10,NULL,ntn);
            
            pthread_create(&thread_id[0], NULL, thread_routine, (void *)0 );
            pthread_create(&thread_id[1], NULL, thread_routine, (void *)1 );
            pthread_create(&thread_id[2], NULL, thread_routine, (void *)2 );
            pthread_create(&thread_id[3], NULL, thread_routine, (void *)3 );
            
            pthread_join(thread_id[0],NULL);
            pthread_join(thread_id[1],NULL);
            pthread_join(thread_id[2],NULL);
            pthread_join(thread_id[3],NULL);
            
            pthread_barrier_destroy(&barrier1);
            pthread_barrier_destroy(&barrier2);
            pthread_barrier_destroy(&barrier3);
            pthread_barrier_destroy(&barrier4);
            pthread_barrier_destroy(&barrier5);
            pthread_barrier_destroy(&barrier6);
            pthread_barrier_destroy(&barrier7);
            pthread_barrier_destroy(&barrier8);
            pthread_barrier_destroy(&barrier9);
            pthread_barrier_destroy(&barrier10);
            
        }
        
        
    }else{
        ntn = 16;
        int iters=4; //(sizeof(unsigned int)*8/log_radix);
        
        for(niter = 0; niter < iters; niter+=2) {
            
            pthread_barrier_init(&barrier1,NULL,ntn);
            pthread_barrier_init(&barrier2,NULL,ntn);
            pthread_barrier_init(&barrier3,NULL,ntn);
            pthread_barrier_init(&barrier4,NULL,ntn);
            pthread_barrier_init(&barrier5,NULL,ntn);
            pthread_barrier_init(&barrier6,NULL,ntn);
            pthread_barrier_init(&barrier7,NULL,ntn);
            pthread_barrier_init(&barrier8,NULL,ntn);
            pthread_barrier_init(&barrier9,NULL,ntn);
            pthread_barrier_init(&barrier10,NULL,ntn);
            
            
            pthread_create(&thread_id[0], NULL, thread_routine, (void *)0 );
            pthread_create(&thread_id[1], NULL, thread_routine, (void *)1 );
            pthread_create(&thread_id[2], NULL, thread_routine, (void *)2 );
            pthread_create(&thread_id[3], NULL, thread_routine, (void *)3 );
            pthread_create(&thread_id[4], NULL, thread_routine, (void *)4 );
            pthread_create(&thread_id[5], NULL, thread_routine, (void *)5 );
            pthread_create(&thread_id[6], NULL, thread_routine, (void *)6 );
            pthread_create(&thread_id[7], NULL, thread_routine, (void *)7 );
            pthread_create(&thread_id[8], NULL, thread_routine, (void *)8 );
            pthread_create(&thread_id[9], NULL, thread_routine, (void *)9 );
            pthread_create(&thread_id[10], NULL, thread_routine, (void *)10 );
            pthread_create(&thread_id[11], NULL, thread_routine, (void *)11 );
            pthread_create(&thread_id[12], NULL, thread_routine, (void *)12 );
            pthread_create(&thread_id[13], NULL, thread_routine, (void *)13 );
            pthread_create(&thread_id[14], NULL, thread_routine, (void *)14 );
            pthread_create(&thread_id[15], NULL, thread_routine, (void *)15 );
            
            pthread_join(thread_id[0],NULL);
            pthread_join(thread_id[1],NULL);
            pthread_join(thread_id[2],NULL);
            pthread_join(thread_id[3],NULL);
            pthread_join(thread_id[4],NULL);
            pthread_join(thread_id[5],NULL);
            pthread_join(thread_id[6],NULL);
            pthread_join(thread_id[7],NULL);
            pthread_join(thread_id[8],NULL);
            pthread_join(thread_id[9],NULL);
            pthread_join(thread_id[10],NULL);
            pthread_join(thread_id[11],NULL);
            pthread_join(thread_id[12],NULL);
            pthread_join(thread_id[13],NULL);
            pthread_join(thread_id[14],NULL);
            pthread_join(thread_id[15],NULL);
            
            pthread_barrier_destroy(&barrier1);
            pthread_barrier_destroy(&barrier2);
            pthread_barrier_destroy(&barrier3);
            pthread_barrier_destroy(&barrier4);
            pthread_barrier_destroy(&barrier5);
            pthread_barrier_destroy(&barrier6);
            pthread_barrier_destroy(&barrier7);
            pthread_barrier_destroy(&barrier8);
            pthread_barrier_destroy(&barrier9);
            pthread_barrier_destroy(&barrier10);
            
            
        }
        
        
    }
}







/*********************************************************************
 * register_multithread_functions - Register all of your different versions
 *     of the multithread kernel with the driver by calling the
 *     add_multithread_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_multithread_functions(){
    add_multithread_function(&naive_multithread, naive_multithread_descr);   
    add_multithread_function(&multithread, multithread_descr);   
    /* ... Register additional test functions here */
}

