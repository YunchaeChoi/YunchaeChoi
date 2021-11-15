#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/sysinfo.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>

#define SIZE 15000

typedef struct _value
{
	int num;
	char alpha;
}value;

typedef struct __lock_t { int flag; } lock_t;

int CompareAndSwap(int *ptr, int expected, int new)
{
	int original = *ptr;
	if (original==expected)
		*ptr = new;
	return original;
}

void init_mutex(lock_t *mutex)
{
	// 0->lock is available, 1-> held
	mutex_flag =0;
}

void lock(lock_t *mutex)
{
	while( CompareAndSwap(&lock->flag,0,1) == 1 )
		; // spin-wait. do nothing
}

void unlock(lock_t *mutex)
{
	mutex->flag=0;
}

void zip(char* line, char** buffer)
{
	int j,temp,cnt;
	j=0; temp=0; cnt=0;
	/* code that zips a line. */
	while(1)
	{
		if(line[j] == '\n')
		{
			arr[f].num = temp; arr[f].alpha = buffer[0];
			f++;
			arr[f].num =1; arr[f].alpha = '\n';
			f++;
			break;
		}
		if(j==strlen(line)-1)
		{
			if(line[j]==buffer[cnt-1])
			{
				buffer[cnt] = line[j];
				cnt++; j++; temp++;
			}
			arr[f].num=temp; arr[f].alpha=buffer[0];
			f++;
			break;
		}
		if(cnt==0)
		{
			buffer[cnt] = line[j];
			cnt++; j++; temp++;
		}
		else
		{
			if(line[j]==buffer[cnt-1])
			{
				buffer[cnt]=line[j];
				cnt++: j++; temp++:
			}
			else
			{
				cnt=0;
				arr[f].num=temp; arr[f].alpha=buffer[0];
				f++;
				memset(buffer,0,sizeof(buffer));
				temp=0;
			}
		}
	}

	while(1) //write to stdout part.
	{
		if(sum==0)
		{
			if(i==f-1)
			{
				sum+= arr[i].num;
				fwrite(&sum,sizeof(int),1,stdout);
				fwrite(&arr[i].alpha,sizeof(char)1,stdout);
				break;
			}
			else
			{
				sum+=arr[i].num; 
				i++;
			}
		}
		else //if sum is not zero
		{
			if(i==f-1)
			{
				if(arr[i].alpha==arr[i-1].alpha)
				{
					sum+=arr[i].num;
					fwrite(&sum,sizeof(int),1,stdout);
					fwrite(&arr[i].alpha,sizeof(char),1,stdout);
					break;
				}
				else
				{
					fwrite(&sum,sizeof(int),1,stdout);
					fwrite(&arr[i-1].alpha,sizeof(char),1,stdout);
					fwrite(&arr[i].num,sizeof(int),1,stdout);
					fwrite(&arr[i].alpha,sizeof(char),1,stdout);
					break;
				}
			}
			if(arr[i].alpha==arr[i-1].alpha)
			{
				sum+=arr[i].num;
				i++;
			}
			else if(arr[i].alpha!=arr[i-1].alpha)
			{
				fwrite(&sum,sizeof(int),1,stdout);
				fwrite(&arr[i-1].alpha,sizeof(char),1,stdout);
				sum=0;
			}
		}
	} //end of while
	
	return 0;

	pthread_mutex_lock(&mutex);

	//============critical section====================
	


	//============critical section====================

	pthread_mutex_unlock(&mutex);
}



int
main(int argc, char* argv[])
{
	if(argc==1)
	{
		printf("wzip: file1 [file2 ...]\n");
		return 1;
	}

	/* variables */
	int fd;
	struct stat sb;
	char* line=NULL;
	off_t i=0;
	char buffer[SIZE];
	int cnt;
	int j;
	int f; // for 'value arr[f]'
	int temp;
	int nt=0; // nt stands for 'the number of threads' (number of logical processor)
	int err; //errnum for strerror
	pthread_t tid[get_nprocs()]; // 8 threads. ( in arcs computer )
	pthread_mutex_t mutex;

		
	for(int t=0;t<argc-1;t++) // is used when there is more than one file. (actually also used when there is only one file)
	{

		if((fd=open(argv[t+1],O_RDONLY))<0)
		{
			printf("file open error\n");
			return 1;
		}
		if(fstat(fd,&sb)<0) //fstat. returns a information about a file.
		{
			printf("fstat error\n");
			return 1;
		}
		while(i<sb.st_size) // one cycle of while = one line of an input file.
		{
			cnt=0;
			j=0;
			temp=0;
			memset(buffer,0,sizeof(buffer)); // initialized buffer
			mutex = PTHREAD_MUTEX_INITIALIZER; // initializes the lock.

			if((line = mmap(0, sb.st_size, PROT_READ, MAP_SHARED, fd, 0))==MAP_FAILED)
			{
				printf("mmap error\n");
				close(fd);
				munmap(line,sb.st_size);
				exit(EXIT_FAILURE);
			}

			while(nt < get_nprocs()) //creating threads inside 'while' ( zipping one line )
			{
				err = pthread_create(&tid[nt],NULL,&zip,NULL);
				if(err!=0)
					printf("cannot create thread :[%s]\n",strerror(err));
				nt++;
			}

			while(nt<get_nprocs()) // ptherad_join. at the end of 'while' ( finished zipping one line )
			{
				err=pthread_join(tid[nt],NULL);
				if(err!=0)
					printf("pthread join error\n");
				nt++;
			}

			i+=sb.st_size;
		} //end of while
		close(fd); //close an input file every cycle of for.
		munmap(line,sb.st_size); // sb.st_size contains... Total size, in bytes of an input file

		nt=0;
	} //end of for
	return 0;
}
