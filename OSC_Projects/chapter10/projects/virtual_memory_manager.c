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

/* if page fault occurs, we need to do
 * 1. read in a 256-byte page from the file BACKING_STORE.bin
 * 2. and store it in an available page frame in physical memory
 * 3. update page table
 * 4. update TLB 
 * 5. read page will be resolved by either the TLB or the page table
 */

// Page : contiguous region of virtual memory
// Frame: contiguous region of physical memory
// Eviction : removing a pge from its frame and potentially writing it to swap table for file system
// Swap Table : where evicted pages are written to in the swap partition


#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "bits.h"
#include "doubly_linked_stack.h"
#include "list.h"

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
    int valid_bit;   // valid-invalid bit. look up the book ( 0 or 1 )
    int present_bit; // indicates whether this page is currently on the memory or not
    int access_bit;  // a.k.a. reference bit. used to track whether a page has been accessed ( useful in page replacement algorithm )
}PAGE_TABLE;

typedef struct _physical_memory_frame
{
    unsigned char frame[FRAME_SIZE]; //256-byte frame size 
}physical_memory_frame;




/* global variables */
tlb TLB[16]; // TLB with 16 entries

PAGE_TABLE page_table[PAGE_TABLE_SIZE]; // page table with 2^8 entries

physical_memory_frame physical_memory[PHYSICAL_MEMORY_SIZE]; // 256frames * 256-byte frame size < Physical Memory >

int frame_table[256]; // global data structure that keeps track of physical frames that are allocated/free

unsigned char file_mapping_table[256]; // keeps track of which memory-mapped files are mapped to which pages

/* fucntions */

void TLB_initialize();

void page_table_initialize();

void init();

tlb* TLB_search(unsigned char page_num);

unsigned char page_walk(unsigned char page_num);

int calculate_phsysical_address(unsigned char frame_num, unsigned char offset);

void TLB_update(unsigned char page_num, unsigned char frame_num);

void page_replacement_FIFO();

void page_replacement_LRU();

int TLB_Find_Empty();

void printf_TLB();

void print_page_table();


/* main */
// Your program will open addresses.txt, 
// read each logical address
// and translate it to its corresponding physical address
// and output the value of the signed byte at the physical address
//
// after completion,
// your program is to report...
// 1. Page-fault rate - the percentage of address references that resulted in page faults
// 2. TLB hit read - the percentage of address references that were resolved in the TLB

int main(int argc, char* argv[])
{
	/* when creating stack, lookup stack_test.c */
    char* strtol_ptr;      // for strtol
    char* input_address;
    char address_buffer[7];
    int logic_addr;     // logical address
    unsigned char logical_address; // logical address. rightmost 16 bits in unsigned char type

	Stack* TLB_stack; // LRU using stack. check OSC page 520.
	StackInit(TLB_stack); 	// stack initialzed.

	Stack* page_stack; // we need two stacks. one for TLB, and one for page table ( this one )
	StackInit(page_stack); // both for LRU algorithm
	
	TLB_initialize();

    FILE* fp;
    if((fp=fopen(argv[1],"r"))==NULL ) // opens BackingStore/addresses.txt
    {
        printf("file open error.\n");
        exit(EXIT_FAILURE);
    }
    while(!feof(fp)) //feof returns non-zero value if EOF
    {
        input_address=fgets(address_buffer,sizeof(address_buffer),fp);
        //logic_addr=(int)strtol(input_address,&strtol_ptr,10);  // logical address acquired. (int type)
        logic_addr=atoi(input_address);

        unsigned char page_number = extract_page_number(logic_addr);
        unsigned char offset= extract_offset(logic_addr);

    }
    return 0;
}

void init()
{
    TLB_initialize();
    page_table_initialize();
}

void TLB_initialize()
{
    tlb* tlb_ptr;
    for(int i=0;i<TLB_SIZE;i++)
    {
        tlb_ptr=&TLB[i];
        tlb_ptr->valid_bit=TLB_EMPTY;
		tlb_ptr->access_bit=TLB_not_refereced;
    }
}

void page_table_initialize()
{
    PAGE_TABLE** page_table_ptr;
    for(int i=0;i<PAGE_TABLE_SIZE;i++)
    {
        page_table_ptr = &page_table[i];
        page_table_ptr->valid_bit=0;
        page_table_ptr->access_bit=0;
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
    tlb* tlb_ptr = NULL; 
    return tlb_ptr; // means TLB_MISS
}

unsigned char page_walk(unsigned char page_num) // if TLB miss -> check page table(traversal) / page walk / return value is physical frame number
{
    if(page_table[page_num].valid_bit==0)
    {
        /* page fault */
        /* page fault hadling function - page replacemnet policy */
    }
    return page_table[page_num].frame_num; // return frame number
}
/*
void page_replacement_FIFO() 
{

}


void page_replacement_LRU(unsigned char page_num, FILE* fp) // most recently used page is always at the top of the stack. So bottom node of the stack will be the victim frame  ( least recently used )
{
	 //need to look up backingstore.bin  
}


void TLB_replacement_LRU(unsigned char page_num, unsigned char frame_num, Stack* stack) // these arguments are new PTE to be place in the TLB
{
	 //the bottom Node of the stack will be the victim 
}
*/


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
		if(TLB[TLB_idx].valid_bit == TLB_EMPTY)
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

void TLB_update(unsigned char page_num, unsigned char frame_num) 
{                                                                // there are two cases when TLB must be updated
	if(TLB_Find_Empty() != TLB_FULL) // TLB initialization step        // 1. at first, TLB entries are empty. so they must be initialized
	{                                                            // 2. when TLB miss occurs, least recently used TLB must be replaced(LRU) and new TLB will be pushed into the top of the stack
		int TLB_idx = TLB_Find_Empty();
		tlb* tlb_ptr = &TLB[TLB_idx];
		tlb_ptr->page_num=page_num;
		tlb_ptr->frame_num=frame_num;
		tlb_ptr->valid_bit=TLB_SET;
	}
	else // if TLB entries are full
	{
		/* TLB replacement policy */
	}
}
/*

void TLB_to_PFN(unsigned char page_num, tlb* tlb_ptr, Stack* TLB_stack) // if TLB hit, return physical frame number from TLB
{
    tlb_ptr->access_bit=TLB_referenced
	
	 //push this page_num to top of the stack
	 //if this page_num already exists in the stack,
	 //pop it and push it to the top of the stack 

	
	return tlb_ptr->frame_num;
}
*/

void print_page_table()
{
    int i=0;
    printf("---------------------PAGE_TABLE--------------\n");
    for(;i<PAGE_TABLE_SIZE;i++)
    {
        printf("%d | %d\n",i,page_table[i].frame_num);
    }
}

void print_TLB()
{
    int i=0;
    printf("------------------TLB---------------------\n");

    for(;i<TLB_SIZE;i++)
    {
        printf("%d | %d\n",TLB[i].page_num,TLB[i].frame_num);
    }
    printf("\n");
    return ;
}
