/*
 * We need, 
 * 1. 2^8 entreis in the page table
 * 2. Page size of 2^8 bytes
 * 3. 16 entries in the TLB
 * 4. Frame size of 2^8 bytes
 * 5. 256 frames
 * 6. Physical memory of 65,636(2^16) bytes (256 frames * 256-byte frame size)
*/

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <stdint.h>
#include <errno.h>
#include "show_bits.h"

#define TRUE 1
#define FALSE 0

typedef struct _tlb
{
    unsigned char page_num;
    unsigned char frame_num;
}tlb;

/* global variables */
tlb TLB[16]; // TLB with 16 entries
tlb_pointer TLB_pointer[16];
unsigned char page_table[256]; // page table with 2^8 entries


/* fucntions */

void intialize_TLB();
void TLB_hit();
tlb TLB_search(unsigned char page_num);
void calculate_phsysical_memory(tlb *ptr, unsigned char offset);

int main(int argc, char* argv[])
{
    char *str_ptr;
    int logic_addr; // logical address
    logic_addr=(int)strtol(argv[1],&str_ptr,10);



    printf("page num: %#x\n",extract_page_number(logic_addr));
    printf("offset: %#x\n",extract_offset(logic_addr));
}


void intialize_TLB()
{
    for(int i=0;i<16;i++)
    {
        TLB[i]->page_num=TLB_EMPTY;
        TLB[i]->frame_num=TLB_EMPTY;
    }
}

tlb* TLB_search(unsigned char page_num)
{
    for(int i=0;i<16;i++)
    {
        if(TLB[i]->page_num == page_num) 
        {
            tlb *ptr = malloc(sizeof(tlb));
            ptr = TLB[i];
            return ptr;
        }
    }
    return NULL;
}

void calculate_phsysical_memory(tlb *ptr, unsigned char offset) // if TLB_search(val)!=NULL means, TLB hit
{

}

void demand_paging()
{

}
