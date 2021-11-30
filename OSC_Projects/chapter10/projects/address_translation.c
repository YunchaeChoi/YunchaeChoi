#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "bits.h"
#include "doubly_linked_stack.h"
#include "list.h"

int main(int argc, char** argv[])
{
    char* strtol_ptr;      // for strtol
    char* input_address;
    char address_buffer[7];
    int logic_addr;     // logical address
    unsigned char logical_address; // logical address. rightmost 16 bits in unsigned char type

    FILE* fp;
    if((fp=fopen(argv[1],"r"))==NULL) // opens BackingStore/addresses.txt
    {
        printf("file open error.\n");
        exit(EXIT_FAILURE);
    }
    while(!feof(fp)) //feof returns non-zero value if EOF
    {
        input_address=fgets(address_buffer,sizeof(address_buffer),fp);
        logic_addr=(int)strtol(input_address,&strtol_ptr,10);  // logical address acquired. (int type)
        logical_address=extract_rightmost_16_bits(logic_addr); // unsigned char address acquired ( 16bits )
        printf("%hhu\n",logical_address);
    }
    fclose(fp);


    return 0;
}
