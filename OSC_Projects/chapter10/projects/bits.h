#ifndef __bits
#define __bits

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdint.h>
#include <errno.h>

#define SHOW(T,V) do { T x = V; print_bits(#T, #V, (unsigned char*) &x, sizeof(x));  } while(0)

#define OFFSET_MASK 0xFF
#define PGN_MASK 0xFF00 // page number mask
#define REMAIN_BITS_MASK 0xFFFF

/* 32-bit logical address */

unsigned char extract_offset(int logical_address);
unsigned char extract_page_number(int logical_address);
unsigned char extract_remain_bits(int logical_address);


unsigned char extract_offset(int logical_address)
{
    return logical_address & OFFSET_MASK;
    // using printf("%#x",extract_offset(logic_addr)) , can print in hexadecimal (e.g., 0xff)
}

unsigned char extract_page_number(int logical_address)
{
    return (logical_address & PGN_MASK)>>8;
}
/*
unsigned char extract_rightmost_16_bits(int logical_address) 
{
    return (logical_address & REMAIN_BITS_MASK);
}
*/
#endif
