/*
 
-------------------------------------------------------------------
             < TLB CONTROL FLOW ALGORITHM >

	 VPN = (VirtualAddress & VPN_MASK) >> SHIFT
	 (Success, TlbEntry) = TLB_Lookup(VPN)
	 if (Success == True) // TLB Hit
	 	if (CanAccess(TlbEntry.ProtectBits) == True)
	 		Offset = VirtualAddress & OFFSET_MASK
	 		PhysAddr = (TlbEntry.PFN << SHIFT) | Offset
	 		Register = AccessMemory(PhysAddr)
	 	else
	 		RaiseException(PROTECTION_FAULT)
	 else // TLB Miss
	 		RaiseException(TLB_MISS)

-------------------------------------------------------------------

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
#include "doubly_linked_stack.h"

#define TRUE 1
#define FALSE 0

#define TLB_EMPTY 0
#define TLB_SET 1

#define TLB_HIT 1
#define TLB_MISS 0

#define PAGE_FAULT 0

#define PAGE_TABLE_SIZE 256
#define TLB_SIZE 16
#define PHYSICAL_MEMORY_SIZE 256
#define FRAME_SIZE 256


typedef struct _tlb
{
    int is_set; // TLB_EMPTY or TLB_SET // may have to rename it to valid bit. not sure
    unsigned char page_num;
    unsigned char frame_num;
    int access_bit; // reference bit. useful in page replacement policy
}tlb;

typedef struct _PAGE_TABLE
{
    unsigned char frame_num;
    int valid_bit; // valid-invalid bit. look up the book ( 0 or 1 )
    int access_bit; // a.k.a. reference bit. used to track whether a page has been accessed ( useful in page replacement algorithm )
}PAGE_TABLE;

typedef struct _physical_memory_frame
{
    unsigned char frame[FRAME_SIZE]; //256-byte frame size 
}physical_memory_frame;


/* global variables */
tlb TLB[16]; // TLB with 16 entries

PAGE_TABLE page_table[PAGE_TABLE_SIZE]; // page table with 2^8 entries

physical_memory_frame physical_memory[PHYSICAL_MEMORY_SIZE]; // 256frames * 256-byte frame size - Physical Memory

/* fucntions */

void intialize_TLB();

tlb* TLB_search(unsigned char page_num);

unsigned char VPN_to_PFN(unsigned char page_num);

int calculate_phsysical_address(tlb *ptr, unsigned char offset);

void TLB_update(unsigned char page_num, unsigned char frame_num);

int TLB_look_up(unsigned char page_num);

void page_replacement_FIFO();

void page_replacement_LRU();


/* main */

int main(int argc, char* argv[])
{
	/* when creating stack, lookup stack_test.c */

    char* strtol_ptr;      // for strtol
    char* input_address;
    char address_buffer[7];
    int logic_addr;     // logical address

	LinkedListStack* stack; // LRU using stack. check OSC page 520.
	stack = stack_init();

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
    return 0;
}

void intialize_TLB()
{
    tlb* tlb_ptr;
    for(int i=0;i<16;i++)
    {
        tlb_ptr=&TLB[i];
        tlb_ptr->is_set=TLB_EMPTY;
    }
}

tlb* TLB_search(unsigned char page_num)
{
    for(int i=0;i<16;i++)
    {
        if(TLB[i].page_num == page_num) 
        {
            //tlb *ptr = malloc(sizeof(tlb));
            //ptr = TLB[i];
            return &TLB[i];
        }
    }
    return NULL;
}

unsigned char VPN_to_PFN(unsigned char page_num) // if TLB miss. check page table(traversal) / page walk / return value is physical frame number
{
    if(page_table[page_num].valid_bit==0)
    {
        /* page fault */
        /* page fault hadling function */
    }
    return page_table[page_num].frame_num;
}
void page_replacement_FIFO() // TLB 와 page table 둘 다 같은 page replacement 함수를 써야 하는 지는 고민 중.
{
}

void page_replacement_LRU()
{
}

int calculate_phsysical_address(unsigned char frame_num, unsigned char offset) // calculating physical address may not be needed. getting frame number and offset may be final step.
{
    int physical_address = 0 ; // initialization

    physical_address+=(int)offset;
    physical_address+=(int)(frame_num<<8);
    //physical_address+=(int)(remain_bits<<16);

    return physical_address;
}

void TLB_update(unsigned char page_num, unsigned char frame_num) // if TLB miss, TLB must be updated
{
    /* need to use replacement policy */
}

int TLB_look_up(unsigned char page_num)
{
    for(int i=0;i<TLB_SIZE;i++)
    {
        if(TLB[i].is_set==TLB_SET && TLB[i].page_num == page_num)
        {
            return TLB_HIT;
        }
    }
    return TLB_MISS;
}
