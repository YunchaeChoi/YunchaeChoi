#include <stdio.h>
#include <cuda.h>
#include <cuda_runtime.h>

__global__ void add(int *a, int *b, int *c, int n)
{
    int index = threadIdx.x + blockIdx.x * blockDim.x;
    if (index < n)
        c[index] = a[index] + b[index];
}


void random_ints(int* a, int N)
{
   int i;
   for (i = 0; i < N; ++i)
       a[i] = rand();
}

#define N (2048*2048)
#define THREADS_PER_BLOCK 512

int main(void)
{
    int *a,*b,*c;
    int *d_a, *d_b, *d_c;
    int size = N*sizeof(int);

    cudaMalloc((void **)&d_a, size);
    cudaMalloc((void **)&d_b, size);
    cudaMalloc((void **)&d_c, size);

    a=(int *)malloc(size); random_ints(a,N);
    b=(int *)malloc(size); random_ints(b,N);
    c=(int *)malloc(size);

    *a=2;
    *b=5;
    printf("%d %d\n",*a,*b);

    cudaMemcpy(d_a, a, size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_b, b, size, cudaMemcpyHostToDevice);

    add<<<(N +M-1)/M, M>>>(d_a,d_b,d_c,N);

    cudaMemcpy(c, d_c, size, cudaMemcpyDeviceToHost);

    printf("c : %d\n",*c);

    free(a); free(b); free(c);

    cudaFree(d_a); 
    cudaFree(d_b); 
    cudaFree(d_c); 

    return 0;
}
