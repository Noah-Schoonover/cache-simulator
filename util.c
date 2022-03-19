/*************************************************************************
*      	filename:  util.c
*   	description:  utility/helper functions
*
*       author:  Schoonover, Noah
*
*	    class:       CSE 331
*    	instructor:  Tong
*    	assignment:  Lab #2
*       due:  	     3/21/2022
*************************************************************************/

#include "util.h"

/**
 * prints an unsigned 32 bit integer in binary format
 * adapted from:
 * https://www.geeksforgeeks.org/binary-representation-of-a-given-number/
 * @param n  the unsigned integer to print
 */
void printbin(uint32_t n)
{
    uint32_t i;
    for (i = 1 << 31; i > 0; i = i / 2)
        (n & i) ? printf("1") : printf("0");
}
