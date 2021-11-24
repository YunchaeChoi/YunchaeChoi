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

#define SWAP(a, b) {size_t temp=a;a=b;b=temp;}

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

void request_memory(char* process,size_t bytes,char strategy);

void compact_memory();

void release_memory(char* process);

void display_memory();

size_t block_size(MemBlock* hole);

/* global variables */

size_t mem_size=0;  // must be initialized to (size_t)argv[1]
MemBlock *mem;

int 
main(int argc, char* argv[])
{
	if(argc==1 || argc>2) exit(EXIT_FAILURE); // need one argument when executing this code

	sscanf(argv[1], "%zu" , &mem_size); // %zu is used especially for size_t.
	mem = make_block(0, mem_size-1,"",NULL,NULL); // initializes memory

	/* variables */
	char* line =NULL;
	size_t len;
	ssize_t nread;
	char* arg[10];
	int an; // argument number. how much arguments entered in prompt line
	char* strtol_ptr;

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
			request_memory(arg[1],(size_t)strtol(arg[2],&strtol_ptr,10),arg[3][0]); // RQ P0 40000 W
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
			//compact_memory();
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

size_t block_size(MemBlock* hole)
{
    return hole->hi - hole->lo +1 ;
}

void request_memory(char* process, size_t bytes, char strategy) // RQ P0 40000 W
{
	MemBlock *hole = mem; // we need to find an empty hole
	switch(strategy)
	{
		case 'F' : // First Fit
			{
				while(hole)
				{
					if(hole->process==NULL && hole->hi - hole->lo +1 >= bytes) // free block big enough to fit requsted memory
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
                    if(cursor->hi-cursor->lo+1 >= bytes)
                    {
                        if(cursor->hi-cursor->lo+1 > hole->hi-hole->lo+1 && cursor->process==NULL)
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
					if(cursor->hi-cursor->lo+1 >= bytes)
                    {
                        if(cursor->hi-cursor->lo+1 < hole->hi-hole->lo+1 && cursor->process==NULL)
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
                exit(EXIT_FAILURE);
                break;
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
        return;
    }
    hole->next = make_block(hole->lo+bytes,hole->hi,"",hole,hole->next); // make hole->name a process and hole->next a free block. starting hole->lo+bytes
    hole->hi = hole->lo+bytes-1;
}

MemBlock* find_head_free(MemBlock* proc) // find lowest memory of free block, and big enough to contain process
{
    MemBlock* ret=mem;
    while(ret)
    {
        if(ret->process==NULL && ret->hi-ret->lo+1 >= proc->hi-proc->lo+1)
            break;
        ret=ret->next;
    }
    return ret;
}

MemBlock* find_proc()
{
    MemBlock* ret=mem;
    while(ret)
    {
        if(ret->process)
            break;
        ret=ret->next;
    }
    return ret;
}

void compact_memory() // will move all existing processes to low memory
{
    MemBlock* proc; // existing memory allocated to a process
    MemBlock* head;

    while(head->next!=NULL)
    {
        proc=find_proc();
        head=find_head_free(proc);
        if(head->next==proc)
        {    
            free(head->process);
            head->process=NULL;

            head->process=malloc(sizeof(char)*(strlen(proc->process)+1));
            strcpy(head->process,proc->process);

            free(proc->process);
            proc->process=NULL;
            
            size_t proc_size = block_size(proc);
            size_t head_size = block_size(head);

            SWAP(head->lo,proc->lo);

            proc->hi = proc_size +proc->lo -1;
            head->hi = head_size +head->lo -1; 
        }
    }
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
