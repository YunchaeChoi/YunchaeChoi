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

int request_memory(char* process,int bytes,char strategy);

void compact_memory();

void release_memory(char* process);

void display_memory();

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
			request_memory(arg[1],(int)strtol(arg[2],&strtol_ptr,10),arg[3][0]); // RQ P0 40000 W
		}

		if(strcmp(arg[0],"RL")==0)
		{
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

int request_memory( char* process ,int bytes, char strategy )
{
	MemBlock * hole =NULL;
	MemBlock *cursor=NULL;
	switch(strategy)
	{
		case 'F' :
			hole=mem;
			while(hole)
			{
				if(hole->process==NULL && (hole->hi - hole->lo +1) >=bytes)
					break;
				hole=hole->next;
			}
			break;
		case 'B' :
			cursor = mem;
			size_t min_size = -1;
			while(cursor)
			{
				size_t hole_size = (cursor->hi - cursor->lo +1);
				if(cursor->process==NULL && bytes<=hole_size && hole_size < min_size)
				{
					min_size = hole_size;
					hole= cursor;
				}
				cursor = cursor->next;
			}
			break;
		case 'W' :
			cursor = mem;
			size_t max_size= bytes-1;
			while(cursor)
			{
				size_t hole_size = (cursor->hi - cursor->lo +1);
				if(cursor->process==NULL && bytes<=hole_size && hole_size > max_size)
				{
					max_size = hole_size;
					hole=cursor;
				}
				cursor=cursor->next;
			}
			break;
		default:
			printf("Unknown strategy\n");
			exit(EXIT_FAILURE);
	}
	if(!hole || hole->process!=NULL) // if hole is NULL, or name(process) of hole is not NULL
	{
		printf("No available memory to allocate\n");
		exit(EXIT_FAILURE);
	}
	hole->process = malloc(sizeof(char)*(strlen(process)+1));
	strcpy(hole->process,process);

	if(hole->hi - hole->lo +1 == bytes ) // ?
	{
		return 0;
	}

	hole->next = make_block(hole->lo+bytes, hole->hi, "", hole, hole->next);
	hole->hi = hole->lo +bytes -1;
	return 0;
	
}

void release_memory(char* process)
{
	int is_released=0;

	MemBlock *target = mem;
	while(target)
	{
		if(target->process && strcmp(target->process,process)==0)
		{
			free(target->process);
			target->process=NULL;
			is_released=1;
		}
		
		if(!target->process && target->prev && !target->prev->process)
		{
			MemBlock *temp = target->prev;
			target->prev = temp->prev;
			if(temp->prev)
			{
				temp->prev->next= target;
			}
			target->lo = temp->lo;
			free(temp);

			if(!target->prev)
				temp->prev->next = target;
			target->lo=temp->lo;
			free(temp);
		}
		
		if(!target->prev)
			mem=target;
		target=target->next;
	}

	if(!is_released)
	{
		printf("No memory released\n");
	}
}


MemBlock *make_block(size_t lo, size_t hi, char* process, MemBlock *prev, MemBlock *next)
{
	MemBlock *ret = malloc(sizeof(MemBlock));
	if(!ret) // if ret is NULL
	{
		exit(EXIT_FAILURE);
	}

	ret->lo =lo; ret->hi =hi;

	if(strlen(process)!=0)
	{
		ret->process = malloc((strlen(process)+1)*sizeof(char));
		strcpy(ret->process,process);
	} 
	else 
	{
		ret->process = NULL;
	}
	
	ret->prev = prev; ret->next = next;

	if(prev) 
	{
		prev->next = ret;
	}
	if(next)
	{
		next-> prev = ret;
	}
	return ret;

}

void compact_memory()
{
	MemBlock *hole = mem;
	MemBlock *proc = mem;
	while(proc)
	{
		if(proc && !proc->process)
			proc=proc->next;
	}
	while(hole)
	{
		if(!hole->process) // if hole is not NULL and name of the hole is NULL > means its free frame(hole)
		{
			if(hole->next->process) // if hole's next is allocated.
			{
				MemBlock *temp = hole->next;
				temp->prev = hole->prev;
				temp->next = hole->next->next;
			}
		}
	}
}

void display_memory() // STAT
{
	MemBlock *cursor = mem;
	while(cursor)
	{
		printf("Address [%06zu : %06zu] ",cursor->lo,cursor->hi);
		if(cursor->process)
		{
			printf("Process %s\n",cursor->process);
		}
		else
		{
			printf("Unused\n");
		}
		cursor=cursor->next;
	}
}
