/*
This file is prototype of final code.
It's objective : to successfully run without TLB. Only with page table.
Will add TLB after the success of this code.
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

#define TRUE 2
#define FALSE 0

#define PAGE_FAULT 0
#define PAGE_TABLE_EMPTY -1

#define PAGE_TABLE_SIZE 256
#define TLB_SIZE 16
#define PHYSICAL_MEMORY_SIZE 256
#define FRAME_SIZE 256

typedef struct _PAGE_TABLE
{
    signed char frame_num;
    int valid_bit;   // valid-invalid bit. look up the book ( 0 or 1 )
    int access_bit;  // a.k.a. reference bit. used to track whether a page has been accessed ( useful in page replacement algorithm )
}PAGE_TABLE;

typedef struct _physical_memory_frame
{
    unsigned char frame[FRAME_SIZE]; //256-byte frame size 
}physical_memory_frame;

/* global variables */

PAGE_TABLE page_table[PAGE_TABLE_SIZE]; // page table with 2^8 entries

physical_memory_frame physical_memory[PHYSICAL_MEMORY_SIZE]; // 256frames * 256-byte frame size < Physical Memory >

int frame_table[256]; // global data structure that keeps track of physical frames that are allocated/free

unsigned char file_mapping_table[256]; // keeps track of which memory-mapped files are mapped to which pages

/* fucntions */

void page_table_initialize();

void init();

unsigned char page_walk(unsigned char page_num);

int calculate_phsysical_address(unsigned char frame_num, unsigned char offset);

void page_replacement_FIFO();

void page_replacement_LRU();

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

	init();
	print_page_table();

    FILE* fp;
    if((fp=fopen(argv[1],"r"))==NULL ) // opens BackingStore/addresses.txt
    {
        printf("file open error.\n");
        exit(EXIT_FAILURE);
    }
    while(!feof(fp)) //feof returns non-zero value if EOF
    {
        input_address=fgets(address_buffer,sizeof(address_buffer),fp);
		if(input_address==NULL) // EOF
			break;

        logic_addr=atoi(input_address);

        unsigned char page_number = extract_page_number(logic_addr);
        unsigned char offset= extract_offset(logic_addr);

		

//		printf("page_num is: %hhu\n",page_number);
//		printf("offset is: %hhu\n",offset);

    } 
    return 0;
}

void init()
{
    page_table_initialize();
}

void page_table_initialize()
{
    PAGE_TABLE* page_table_ptr;
    for(int i=0;i<PAGE_TABLE_SIZE;i++)
    {
        page_table_ptr = &page_table[i];
		page_table_ptr->frame_num = PAGE_TABLE_EMPTY;
        page_table_ptr->valid_bit=0;
        page_table_ptr->access_bit=0;
    }
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

int calculate_phsysical_address(unsigned char frame_num, unsigned char offset) 
{
    int physical_address = 0 ; // initialization

    physical_address+=(int)offset;
    physical_address+=(int)(frame_num<<8);
    return physical_address;
}


void print_page_table()
{
    int i=0;
    printf("---------------------PAGE_TABLE--------------\n");
    for(;i<PAGE_TABLE_SIZE;i++)
    {
		if(page_table[i].frame_num == PAGE_TABLE_EMPTY)
		{
			printf("%d | Empty\n",i);
		}
		else
        	printf("%d | %d\n",i,page_table[i].frame_num);
    }
}

void paging(unsigned char page_num) // page fault handling_for_page_table
{
	/* if a logical address with page number 15 resulted in a page fault,
	   your program would read in page 15 from BACKING_STROE
	   and store it in a page frame in physical memory
	*/


}
