#include <stdio.h>
#include <cuda.h>
#include <cuda_runtime.h>
#include <unistd.h>

__global__ void hello()
{
    printf("hello world from GPU\n");
}

int main()
{
    hello<<<1,4>>>();
    cudaDeviceSynchronize();
    cudaDeviceReset();
    printf("CPU\n");

    return 0;
}
