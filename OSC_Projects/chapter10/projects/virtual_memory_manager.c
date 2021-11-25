/*
 * We need, 
 * 1. 2^8 entreis in the page table
 * 2. Page size of 2^8 bytes
 * 3. 16 entries in the TLB
 * 4. Frame size of 2^8 bytes
 * 5. 256 frames
 * 6. Physical memory of 65,636(2^16) bytes (256 frames * 256-byte frame size)
*/

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "bits.h"

#define TRUE 1
#define FALSE 0

#define TLB_EMPTY 0
#define TLB_SET 1

#define TLB_HIT 1
#define TLB_MISS 0

#define PAGE_TABLE_SIZE 256
#define TLB_SIZE 16

typedef struct _tlb
{
    int is_set; // TLB_EMPTY or TLB_SET
    unsigned char page_num;
    unsigned char frame_num;
}tlb;

typedef struct _PAGE_TABLE
{
    unsigned char frame_num;
    int valid; // valid-invalid bit. look up the book ( 0 or 1 )
}PAGE_TABLE;

/* global variables */
tlb TLB[16]; // TLB with 16 entries

PAGE_TABLE page_table[PAGE_TABLE_SIZE]; // page table with 2^8 entries

/* fucntions */

void intialize_TLB();
void TLB_hit();
tlb* TLB_search(unsigned char page_num);
unsigned char page_walk(unsigned char page_num);
int calculate_phsysical_memory(tlb *ptr, unsigned char offset);
void TLB_set(unsigned char page_num, unsigned char frame_num);
int TLB_check(unsigned char page_num);

int main(int argc, char* argv[])
{
    char* strtol_ptr;      // for strtol
    char* input_address;
    char address_buffer[7];
    int logic_addr;     // logical address
    
    FILE* fp;
    if( (fp=fopen(argv[1],"r"))==NULL )
    {
        printf("file open error.\n");
        exit(EXIT_FAILURE);
    }
    while(!feof(fp)) //feof returns non-zero value if EOF
    {
        input_address=fgets(address_buffer,sizeof(address_buffer),fp);
        logic_addr=(int)strtol(input_address,&strtol_ptr,10);
        printf("%d\n",logic_addr);
    }
}

void intialize_TLB()
{
    for(int i=0;i<16;i++)
    {
        TLB[i]->is_set=TLB_EMPTY;
    }
}

tlb* TLB_search(unsigned char page_num)
{
    for(int i=0;i<16;i++)
    {
        if(TLB[i]->page_num == page_num) 
        {
            //tlb *ptr = malloc(sizeof(tlb));
            //ptr = TLB[i];
            return TLB[i];
        }
    }
    return NULL;
}

unsigned char page_walk(unsigned char page_num) // if TLB miss. check page table(traversal) 
{
    int page_table_idx=0;
    for(;page_table_idx<PAGE_TABLE_SIZE;page_table_idx++)
    {
        if(page_table[page_table_idx]==page_num)
        {
            return page_table[page_table_idx];
        }
    }
    printf("page table miss!\n");
    exit(EXIT_FAILURE);
}


int calculate_phsysical_address(unsigned char page_num, unsigned char offset, unsigned char remain_bits)
{
    int physical_address = 0 ; // initialization

    physical_address+=(int)offset;
    physical_address+=(int)(page_num<<8);
    physical_address+=(int)(remain_bits<<16);

    return physical_address;
}

void TLB_set(unsigned char page_num, unsigned char frame_num)
{
    for(int i=0;i<TLB_SIZE;i++)
    {
        if(TLB[i]->is_set==TLB_EMPTY)
        {
            TLB[i]->page_num=page_num;
            TLB[i]->frame_num=frame_num;
            TLB[i]->is_set=TLB_SET;
            return;
        }
    }
    printf("TLB is already full\n");
    return;
}

int check_TLB(unsigned char page_num)
{
    for(int i=0;i<TLB_SIZE;i++)
    {
        if(TLB[i]->is_set==TLB_SET && TLB[i]->page_num == page_num)
        {
            return TLB_HIT;
        }
        return TLB_MISS;
    }
}


