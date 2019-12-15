#include <stdio.h>
#define _GNU_SOURCE             /* See feature_test_macros(7) */
#include <omp.h>
#include <sys/types.h>
#include <sys/syscall.h>
#define gettid() syscall(SYS_gettid)
#include <sched.h>

#define N 2
int t[N];

int main(){
    omp_set_num_threads(N);
#pragma omp parallel
    {
        int i = omp_get_thread_num();
        cpu_set_t set;
        CPU_ZERO(&set);
        if(i){
            CPU_SET(0, &set);
        }else{
            CPU_SET(4, &set);
        }
        sched_setaffinity(gettid(), sizeof(set), &set);
        while(1){
#pragma omp barrier
            t[i] = 1;
            printf("%d ", t[(i+1)%N]);
#pragma omp barrier
            t[i] = 0;
#pragma omp single
            printf("\n");
        }
    }
    return 0;
}

