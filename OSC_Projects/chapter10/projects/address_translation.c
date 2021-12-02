#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "bits.h"
#include "doubly_linked_stack.h"
#include "list.h"

int main(int argc, char* argv[])
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
    int i=0;
    while(!feof(fp)) //feof returns non-zero value if EOF
    {
        input_address=fgets(address_buffer,sizeof(address_buffer),fp);
        if(input_address==NULL) break;
        logic_addr=atoi(input_address);  // logical address acquired. (int type)
        unsigned char offset = extract_offset(logic_addr);
        unsigned char page = extract_page_number(logic_addr);
        printf("page %d : %hhu\n",i,page);
        printf("offset: %hhu\n",offset);
        i++;
    }
    fclose(fp);
    return 0;
}
