/* description */
// This code allocates memory blocks and link them with doubly-linked list.
// struct _MemBlock contains low, high address and process(name) and prev,next pointer (for linked list).

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

 /* define */

#define SWAP(a, b) {size_t temp=a;a=b;b=temp;} // swap macro

#define TRUE 1
#define FALSE 0


/* struct */

typedef struct _MemBlock
{
	size_t lo, hi;
	char *process;
	struct _MemBlock *prev, *next;
} MemBlock;


/* functions */

void tok(char* line, char* arg[10], int* an); //to make token ( seperate line from getline )

MemBlock *make_block(size_t lo, size_t hi, char* process, MemBlock *prev, MemBlock *next);

void initialize_memory();

int request_memory(char* process,size_t bytes,char strategy);

void compact_memory();

void release_memory(char* process);

void display_memory();

size_t block_size(MemBlock* hole);

MemBlock* find_head_free();

MemBlock* find_proc();

void merge_free_blocks(MemBlock* block,MemBlock* block_merge); // two blocks must be free(not allocated), and not have a name(process), not be NULL

void swap_blocks(MemBlock* head, MemBlock* proc);

int is_there_process(); // return 1 if there are at least one process on the memory. else return 0

int is_there_free_block(); // return 1 if at least one free block. else return 0


/* global variables */
size_t mem_size;  // must be initialized to (size_t)argv[1]
MemBlock *mem;

int 
main(int argc, char* argv[])
{
	if(argc==1 || argc>2)
    {
        printf("not a correct number of arguments\n");
        exit(EXIT_FAILURE);
    }

	sscanf(argv[1], "%zu" , &mem_size); // %zu is used especially for size_t.
    initialize_memory();

	/* variables */
	char* line =NULL;
	size_t len;
	ssize_t nread;
	char* arg[10];
	int an; // argument number. how much arguments entered in prompt line
	char* strtol_ptr;

    char* process_name[20]; // can allocate memory to processes up to 20.
    int proc_idx=0;
    int is_initial_request=TRUE;

	while(1)
	{
		an=0;
		printf("allocator> ");
		if((nread=getline(&line,&len,stdin))==-1)
		{
			printf("getline error or EOF\n");
			exit(EXIT_FAILURE);
		}
		tok(line,arg,&an);
		
		if(strcmp(arg[0],"X")==0) // exit when command is "X".  
		{
			printf("exit...\n");
			exit(EXIT_SUCCESS);
		}

		if(strcmp(arg[0],"RQ")==0) // we have to allocate some amount of memory to a new process
		{
            request_memory(arg[1],(size_t)strtol(arg[2],&strtol_ptr,10),arg[3][0]); // RQ P0 40000 W //
		}

		if(strcmp(arg[0],"RL")==0)
		{
            release_memory(arg[1]);
		}

		if(strcmp(arg[0],"STAT")==0)
		{
			display_memory();
		}
		if(strcmp(arg[0],"C")==0)
		{
			compact_memory();
		}

	}
	free(line);
	return 0;
}

void tok(char* line, char* arg[10], int* an)
{
	assert(line!=NULL);
	char* temp = malloc(strlen(line)*sizeof(char));
	strcpy(temp,line);
	char* token;
	char* newline = strchr(temp,'\n'); // read man strchr
	if(newline)
		*newline=0;
	token = strtok(temp," ");
	*an=0;
	while(token)
	{
		arg[*an] = (char*)malloc(sizeof(char)*((int)strlen(token)+1));
		strcpy(arg[(*an)++],token);
		token = strtok(NULL," ");
	}
	free(temp);
}

MemBlock *make_block(size_t lo, size_t hi, char* process, MemBlock *prev, MemBlock *next)
{
	MemBlock *ret = malloc(sizeof(MemBlock));
	if(!ret)
	{
		printf("malloc failed\n");
		exit(EXIT_FAILURE);
	}
	ret->lo=lo; ret->hi=hi;
	if(strlen(process)!=0)
	{
		ret->process=malloc(sizeof(char)*(strlen(process)+1));
		strcpy(ret->process,process);
	}
	else // for initialization
	{
		ret->process =NULL;
	}
	ret->prev = prev;
	ret->next = next;
	if(prev) // if previous block exists (=not NULL )
	{
		prev->next = ret;
	}
	if(next)
	{
		next->prev= ret;
	}
	return ret;
}

void initialize_memory()
{
    mem = make_block(0, mem_size-1,"",NULL,NULL);
}

size_t block_size(MemBlock* hole)
{
    return hole->hi - hole->lo +1 ;
}

int request_memory(char* process, size_t bytes, char strategy) // RQ P0 40000 W
{
	MemBlock *hole = mem; // we need to find an empty hole
	switch(strategy)
	{
		case 'F' : // First Fit
			{
				while(hole)
				{
					if(hole->process==NULL && block_size(hole) >= bytes) // free block big enough to fit requsted memory
					{
						break;
					}
					hole=hole->next;
				}
                break;
			}
		case 'W' : // worst fit. allocates a processes to the largest free block.
			{
				MemBlock *cursor= mem;
				while(cursor)
				{
                    if(block_size(cursor) >= bytes)
                    {
                        if(block_size(cursor) > block_size(hole) && cursor->process==NULL)
                        {
                            hole = cursor;
                        }
                    }
					cursor=cursor->next;
				}
                break;
			}
		case 'B' : // best fit
			{
				MemBlock *cursor= mem;
				while(cursor)
				{
					if(block_size(cursor) >= bytes)
                    {
                        if(block_size(cursor)< block_size(hole) && cursor->process==NULL)
                        {
                            hole = cursor;
                        }
                    }
                    cursor=cursor->next;
				}
                break;
			}
        default :
            {
                printf("Unknown Strategy\n");
                return 1;
            }
	}
    if(!hole || hole->process!=NULL)
    {
        printf("No available memory to allocate\n");
        exit(EXIT_FAILURE);
    }
    hole->process = malloc(sizeof(char)*(strlen(process)+1));
    strcpy(hole->process,process);

    if(hole->hi-hole->lo+1 == bytes) // no need to make_block(). perfect fit
    {
        return 0;
    }
    hole->next = make_block(hole->lo+bytes,hole->hi,"",hole,hole->next); // make hole->name a process and hole->next a free block. starting hole->lo+bytes
    hole->hi = hole->lo+bytes-1;
    return 0;
}

MemBlock* find_head_free() // find lowest memory(address) of free block
{
    int free_flag=0; // set to 1 if at least one free memory block exists in the memory
    MemBlock* ret=mem;
    while(ret)
    {
        if(ret->process==NULL)
        {
            free_flag=1;
            break;
        }
        ret=ret->next;
    }
    if(free_flag)
        return ret;
    else
        return NULL;
}

int is_there_process()
{
    MemBlock* ret=mem;
    while(ret)
    {
        if(ret->process)
        {
            return 1;
        }
        ret=ret->next;
    }
    return 0;
}

int is_there_free_block()
{
    MemBlock* ret=mem;
    while(ret)
    {
        if(!ret->process)
        {
            return 1;
        }
        ret=ret->next;
    }
    return 0;
}

MemBlock* find_proc()
{
    MemBlock* ret=mem;
    while(ret)
    {
        if(ret->process)
        {
            return ret;
        }
        ret=ret->next;
    }
}

void merge_free_blocks(MemBlock* block,MemBlock* block_merge) // two blocks must be free(not allocated), and not have a name(process), not be NULL
{                                                        // block_merge will be merged into block
    if(block->next==block_merge)
    {
        block->hi=block_merge->hi;
        block->next=block_merge->next;
        free(block_merge);
    }
}

void swap_blocks(MemBlock* head, MemBlock* proc) // swap two blocks 
{
    /* head means free block and proc means existing memory allocated to a certain process */
    if(head->next==proc)
    {    
        head->process=malloc(sizeof(char)*(strlen(proc->process)+1));
        strcpy(head->process,proc->process);

        free(proc->process);
        proc->process=NULL;
        
        size_t proc_size = block_size(proc);
        size_t head_size = block_size(head);

        SWAP(head->lo,proc->lo); //swap starting address ( head, proc )

        proc->hi = proc_size +proc->lo -1;
        head->hi = head_size +head->lo -1;

        head->prev=proc;
        head->next=proc->next;

        if(proc->next)
        {
            proc->next->prev=head;
        }

        proc->next=head;
        proc->prev=head->prev;
        if(head->prev)
        {
            head->prev->next=proc;
        }
        if(proc->next && proc->process==NULL && proc->next->process==NULL) // merge two free block
        {
            merge_free_blocks(proc,proc->next);
        }
    }
    
}


void compact_memory() // will move all existing processes to low memory
{
    if(!is_there_free_block) // there is no free block
    {
        printf("there is no free block to compact\n");
        return; // no need to compact
    }
    if(!is_there_process)
    {
        printf("there is no process on the memory\n");
        return;
    }

    MemBlock* proc; // existing memory allocated to a process
    MemBlock* head; // lowest address of free memory block
    do
    {
        if(is_there_process)
            proc=find_proc();
        else
            break;

        if(is_there_free_block)
            head=find_head_free();
        else
            break;

        swap_blocks(head,proc); // swap free block and existing process
    }while(head->next);
    printf("compact done\n");
}

void release_memory(char* process)
{
    int is_released=0; // set to 1 if memory is successfully released
    MemBlock *target = mem;
    while(target)
    {
        if(strcmp(target->process,process)==0)
        {
            free(target->process);
            target->process=NULL;
            is_released=1;
            break;
        }
        target=target->next;
    }

    if(target->process==NULL && target->prev && target->prev->process==NULL)
    {
        MemBlock* temp=target->prev; // to be freed
        target->lo = temp->hi;
        if(temp->prev)
        {
            temp->prev->next=target;
        }
        target->prev=temp->prev;

        free(temp);
    }

    if(target->process==NULL && target->next && target->next->process==NULL)
    {
        MemBlock* temp=target->next; // to be freed
        target->hi = temp->hi;

        if(temp->next)
        {
            temp->next->prev=target;
        }
        target->next = temp->next;

        free(temp);
    }

    if(!is_released)
    {
        printf("No memory released\n");
        return;
    }
}

void display_memory()
{
	MemBlock *cursor = mem;
	while(cursor)
	{
		printf("Addresses [%zu : %zu] ",cursor->lo,cursor->hi);
		if(cursor->process)
		{
			printf("Process %s\n",cursor->process);
		}
		else
			printf("Unused\n");
		cursor=cursor->next;
	}
}
