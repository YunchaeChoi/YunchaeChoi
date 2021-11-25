#ifndef __show_bits
#define __show_bits

#include <stdio.h>
#include <limits.h>
#include <stdint.h>
#include <errno.h>

#define SHOW(T,V) do { T x = V; print_bits(#T, #V, (unsigned char*) &x, sizeof(x));  } while(0)

#define OFFSET_MASK 0xFF
#define PGN_MASK 0xFF00 // page number mask

#define TLB_EMPTY -1 // because TLB entries are consisted of page and frame number, cannot be -1

unsigned char extract_offset(int logical_address);
unsigned char extract_page_number(int logical_address);

unsigned char extract_offset(int logical_address)
{
    return logical_address & OFFSET_MASK;
    // using printf("%#x",extract_offset(logic_addr)) , can print in hexadecimal (e.g., 0xff)
}

unsigned char extract_page_number(int logical_address)
{
    return (logical_address & PGN_MASK)>>8;
}

#endif
