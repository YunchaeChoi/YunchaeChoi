#include <stdio.h>
#include <limits.h>
#include <stdint.h>
#include <errno.h>

#define OFFSET_MASK 0xff
#define PGN_MASK 0xff
#define PAGE_NUM_MASK

#define SHOW(T,V) do { T x = V; print_bits(#T, #V, (unsigned char*) &x, sizeof(x));  } while(0)

typedef unsigned char bit;

/* fucntions */
bit extract_offset(int logical_address);
bit extract_page_number(int logical_address);

void print_byte_as_bits(char val);
void print_bits(char * ty, char * val, unsigned char * bytes, size_t num_bytes);

int main()
{
    SHOW(int, 17);
    SHOW(int,256);
}


bit extract_offset(int logical_address)
{
    return (logical_address && OFFSET_MASK);
}

bit extract_page_number(int logical_address)
{
    return (logical_address>>2) && PGN_MASK; 
}

void print_byte_as_bits(char val) {
  for (int i = 7; 0 <= i; i--) {
    printf("%c", (val & (1 << i)) ? '1' : '0');
  }
}

void print_bits(char * ty, char * val, unsigned char * bytes, size_t num_bytes) {
  printf("(%*s) %*s = [ ", 15, ty, 16, val);
  for (size_t i = 0; i < num_bytes; i++) {
    print_byte_as_bits(bytes[i]);
    printf(" ");
  }
  printf("]\n");
}
