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

#define TLB_FULL -1

#define TLB_not_refereced 0
#define TLB_referenced 1

#define PAGE_FAULT 0

#define PAGE_TABLE_SIZE 256
#define TLB_SIZE 16
#define PHYSICAL_MEMORY_SIZE 256
#define FRAME_SIZE 256


typedef struct _tlb
{
    int valid_bit; // TLB_EMPTY or TLB_SET. tells the hardware if there is a valid translation present in the entry ( from OSTEP )
    unsigned char page_num;
    unsigned char frame_num;
    int access_bit; // reference bit. useful in page replacement policy
}tlb;

typedef struct _PTE // pte to be pushed into stack ( TLB LRU )
{					// because valid_bit and access_bit don't need to be pushed into the stack
	unsigned char page_num;
	unsigned char frame_num;
}PTE;

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

physical_memory_frame physical_memory[PHYSICAL_MEMORY_SIZE]; // 256frames * 256-byte frame size < Physical Memory >

/* fucntions */

void TLB_initialize();

tlb* TLB_search(unsigned char page_num);

unsigned char page_walk(unsigned char page_num);

int calculate_phsysical_address(tlb *ptr, unsigned char offset);

void TLB_update(unsigned char page_num, unsigned char frame_num);

void page_replacement_FIFO();

void page_replacement_LRU();

int TLB_Find_Empty();


/* main */

int main(int argc, char* argv[])
{
	/* when creating stack, lookup stack_test.c */
    char* strtol_ptr;      // for strtol
    char* input_address;
    char address_buffer[7];
    int logic_addr;     // logical address

	Stack* TLB_stack; // LRU using stack. check OSC page 520.
	TLB_stack = stack_init(); 	// stack initialzed.

	Stack* page_stack; // we need two stacks. one for TLb, and one for page table
	page_stack = stack_init(); // both for LRU algorithm
	
	TLB_initialize();

    FILE* fp;
    if( (fp=fopen(argv[1],"r"))==NULL )
    {
        printf("file open error.\n");
        exit(EXIT_FAILURE);
    }
    while(!feof(fp)) //feof returns non-zero value if EOF
    {
        input_address=fgets(address_buffer,sizeof(address_buffer),fp);
        logic_addr=(int)strtol(input_address,&strtol_ptr,10); // logical address acquired. (int type)
    }
    return 0;
}

void TLB_initialize()
{
    tlb* tlb_ptr;
    for(int i=0;i<16;i++)
    {
        tlb_ptr=&TLB[i];
        tlb_ptr->valid_bit=TLB_EMPTY;
		tlb_ptr->access_bit=TLB_not_refereced;
    }
}

tlb* TLB_search(unsigned char page_num)
{
    for(int i=0;i<16;i++)
    {
        if(TLB[i].valid_bit==TLB_SET && TLB[i].page_num == page_num) 
        {
            tlb* tlb_ptr = &TLB[i]; 
            return tlb_ptr; // means TLB_HIT
        }
    }
    return NULL; // means TLB_MISS
}

unsigned char page_walk(unsigned char page_num) // if TLB miss -> check page table(traversal) / page walk / return value is physical frame number
{
    if(page_table[page_num].valid_bit==0)
    {
        /* page fault */
        /* page fault hadling function - page replacemnet policy */
    }
    return page_table[page_num].frame_num;
}

void page_replacement_FIFO() // TLB 와 page table 둘 다 같은 page replacement 함수를 써야 하는 지는 고민 중.
{

}

void page_replacement_LRU(unsigned char page_num, FILE* fp) // most recently used page is always at the top of the stack. So bottom node of the stack will be the victim frame  ( least recently used )
{
	/* need to look up backingstore.bin */ 

}

void TLB_replacement_LRU(unsigned char page_num, unsigned char frame_num, Stack* stack) // these arguments are new PTE to be place in the TLB
{
	/* the bottom Node of the stack will be the victim */
	PTE
	SPop_Bottom(stack); // pop bottom
	SPush(stack,);
}

int calculate_phsysical_address(unsigned char frame_num, unsigned char offset) 
{
    int physical_address = 0 ; // initialization

    physical_address+=(int)offset;
    physical_address+=(int)(frame_num<<8);
    return physical_address;
}

int TLB_Find_Empty()
{
	int empty=0;
	int TLB_idx=0;
	for(;TLB_idx;TLB_idx++)
		if(TLB[TLB_idx].vaild == TLB_EMPTY)
		{
			empty=1;
			break;
		}
	if(!empty) // TLB entries are full
	{
		return TLB_FULL; // return -1;
	}
	return TLB_idx; // return first met empty TLB_entry index
}

void TLB_update(unsigned char page_num, unsigned char frame_num) // if TLB miss, TLB must be updated and also when TLBs are initially filled
{
	if(TLB_Find_Empty() != -1) // if TLB entries are not full
	{
		TLB_idx = TLB_Find_Empty();
		tlb* tlb_ptr = &TLB[TLB_idx];
		tlb_ptr->page_num=page_num;
		tlb_ptr->frame_num=frame_num;
		tlb_ptr->vaild=TLB_SET;
	}
	else // if TLB entries are full
	{
		/* TLB replacement policy */
	}
}

void TLB_to_PFN(unsigned char page_num, tlb* tlb_ptr, Stack* TLB_stack) // if TLB hit, return physical frame number from TLB
{
	tlb_ptr->access_bit=TLB_referenced
	
	/* push this page_num to top of the stack
	 * if this page_num already exists in the stack,
	 * pop it and push it to the top of the stack */

	
	return tlb_ptr->frame_num;
}
