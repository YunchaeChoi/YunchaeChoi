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
#include <unistd.h>
#include <sys/uio.h>
#include "bits.h"
#include "doubly_linked_stack.h"
#include "list.h"

#define TRUE 1 
#define FALSE 0

#define PAGE_FAULT 0
#define PAGE_TABLE_EMPTY -1

#define PAGE_TABLE_SIZE 256
#define TLB_SIZE 16
#define PHYSICAL_MEMORY_SIZE 256
#define FRAME_SIZE 256
#define SINGLE_PAGE_SIZE 256

typedef struct _PAGE_TABLE
{
    signed char frame_num;
    int valid_bit;   // valid-invalid bit. look up the book ( 0 or 1 )
//    int access_bit;  // a.k.a. reference bit. used to track whether a page has been accessed ( useful in page replacement algorithm ) // we'll come back to this later. not now
}PAGE_TABLE;

typedef struct physical_memory
{
    unsigned char frame[FRAME_SIZE]; //256-byte frame size 
}PHYSICAL_MEMORY;

/* global variables */

PAGE_TABLE page_table[PAGE_TABLE_SIZE]; // page table with 2^8 entries

PHYSICAL_MEMORY physical_memory[PHYSICAL_MEMORY_SIZE]; // 256frames * 256-byte frame size < Physical Memory >

int frame_table[256]; // global data structure that keeps track of physical frames that are allocated/free

unsigned char file_mapping_table[256]; // keeps track of which memory-mapped files are mapped to which pages

/* fucntions */

void page_table_initialize();

void physical_memory_initialize();

void init();

void translate_address(char* line,unsigned char* page_number, unsigned char* offset);

int read_single_byte (unsigned char page_number,unsigned char offset); 

void read_page(unsigned char page_number);

unsigned char page_walk(unsigned char page_num);

int calculate_phsysical_address(unsigned char frame_num, unsigned char offset);

void page_replacement_FIFO();

void page_replacement_LRU();

void print_page_table();

void paging(unsigned char page_num); // page fault handling_for_page_table

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
//    unsigned char logical_address; // logical address. rightmost 16 bits in unsigned char type

	init();
//	print_page_table();

	int fd; // file descriptor to open addresses.txt
	fd = open(argv[1],O_RDONLY);
	if(fd==-1)
	{
			printf("Open failed\n");
			exit(EXIT_FAILURE);
	}
	printf("Opened successfully.\n");

	FILE* fp = fdopen(fd,"r");
	if(fp==NULL)
	{
			printf("fd -> fp conversion failed\n");
			exit(EXIT_FAILURE);
	}

	char* line =NULL;
	size_t len=0;
	ssize_t nread;

	unsigned char page_number;
	unsigned char offset;

	while( (nread = getline(&line, &len, fp)) != -1 )
	{
			translate_address(line, &page_number, &offset);

			if(page_table[page_number].frame_num==PAGE_TABLE_EMPTY)
			{
					paging(page_number);
			}
	}

	free(line);
	fclose(fp);
	close(fd);
    return 0;
}

void init()
{
    page_table_initialize();
	physical_memory_initialize();
}

void page_table_initialize()
{
    PAGE_TABLE* page_table_ptr;
    for(int i=0;i<PAGE_TABLE_SIZE;i++)
    {
        page_table_ptr = &page_table[i];
		page_table_ptr->frame_num = PAGE_TABLE_EMPTY;
        page_table_ptr->valid_bit=0;
//        page_table_ptr->access_bit=0;
    }
}

void physical_memory_initialize() // PHYSICAL_MEMORY_SIZE and FRAME_SIZE are both 256.
{
		PHYSICAL_MEMORY* physical_memory_ptr;
		for(int i=0;i<PHYSICAL_MEMORY_SIZE;i++)
		{
				physical_memory_ptr = &physical_memory[i];
				for(int j=0;j<FRAME_SIZE;j++)
				{
						physical_memory_ptr->frame[j] = 0;
				}
		}
}

void translate_address(char* line,unsigned char* page_number, unsigned char* offset)
{
	int logic_addr = atoi(line);
	
	*page_number = extract_page_number(logic_addr);
	*offset= extract_offset(logic_addr);
}		

int read_single_byte(unsigned char page_number,unsigned char offset) // reads a single byte from a BACKINGSTORE.bin
{								   								     // correct.txt/value = single byte.
	int fd_bin = open("./BACKING_STORE.bin",O_RDONLY);
	if(fd_bin==-1) {printf("binary open failed - read_single_byte\n"); exit(EXIT_FAILURE);}
	char value;
	lseek(fd_bin,page_number*SINGLE_PAGE_SIZE + offset, SEEK_SET);
	if(read(fd_bin,&value,1)== -1) // reads a byte
	{
			printf("read single byte failed\n");
			exit(EXIT_FAILURE);
	}
	
	close(fd_bin);
	return (int)value;
}

void read_page(unsigned char page_number)
{
	int fd_bin = open("./BACKING_STORE.bin",O_RDONLY);
	if(fd_bin==-1) {printf("binary open failed - read_page\n"); exit(EXIT_FAILURE);}
	unsigned char page[SINGLE_PAGE_SIZE];

	lseek(fd_bin,page_number*SINGLE_PAGE_SIZE, SEEK_SET);

	if(read(fd_bin,page,SINGLE_PAGE_SIZE)== -1) // reads a page
	{
			printf("read single page failed\n");
			exit(EXIT_FAILURE);
	}
	
	close(fd_bin);
	PHYSICAL_MEMORY* physical_memory_ptr;
	physical_memory_ptr = &physical_memory[page_number];
	for(int i=0;i<SINGLE_PAGE_SIZE;i++)
	{
			physical_memory_ptr->frame[i] = page[i];
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

void print_physical_memory()
{
		int i=0;
		printf("---------------------phsyical_memory--------------\n");

}

void paging(unsigned char page_number) // page fault handling_for_page_table
{
		// means, page_table[page_numer] is PAGE_TABLE_EMPTY. valid_bit : 0.
		// have to read from BACKING_STORE.bin
		
		int fd; // file decriptor. reads BACKING_STORE.bin ( binary file )

		fd = open("./BACKING_STORE.bin",O_RDONLY);
		if(fd==-1)
		{
				printf("Opening BACKING_STORE.bin : FAIL\n");
				exit(EXIT_FAILURE);
		}

		unsigned char single_page[SINGLE_PAGE_SIZE]; // reads 256-bytes ( one page ) from BACK~.bin

		lseek(fd,SINGLE_PAGE_SIZE * page_number, SEEK_SET); // file offset positioned to- (page_number)th page. (e.g., 15th page)

		read_page(page_number);

		/*
		 * what should go in this part, ...
		 *
		 // done * A page that has been read above,
		 // done * should be stored in physical memory.
		 * And page table should be updated. (and TLB, later)
		 *
		 */


}
