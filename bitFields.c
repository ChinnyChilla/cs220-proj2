#include "bitFields.h"
#include <assert.h>
#include <stdio.h>

int getBit(int bn,unsigned long val) {
	assert(bn >=0 && bn <= 63);
	// TODO:
	//		check to make sure bn is a valid bit position in long (you may assert this is true)
	//		If the bit in val at position bn is 1, return 1
	//		otherwise return 0.
	return ((val >> bn) & 1); // replace this with the correct code
}

void setBit(int bn,int new,unsigned long *val) {
	assert(bn >=0 && bn <= 63);
	unsigned long tempVal = 1UL << bn;
	if (new) {
		*val = *val | tempVal;
	} else {
		*val = *val & ~tempVal;
	}
	// printf("setbit finished %d\n", bn);
	// TODO:
	//		check to make sure bn is a valid bit position in long (you may assert this is true)
	//		if new is true, set the bit in val at bn to 1
	//     otherwise, set the bit in val at bn to 0
}

long getBitFld(int bs,int len,unsigned long val) {
	assert(bs >= 0 && bs <= 63);
	// assert(bs + len <= sizeof(long) * 8);
	unsigned long result = getBitFldU(bs, len, val);
	int sign = getBit(len -1, result);
	// print_bits(val);
	if (sign != 0) {
		unsigned long mask = ~0UL << (len);
		result = result | mask;
	}
	// TODO:
	//		check to make sure bs is a valid bit position in long (you may assert this is true)
	//		check to make sure len is a valid length for a subfield that starts at bs (you may assert this is true)
	// 		perform an unsigned getBitFldU to extract the unsigned version of the fs22eld
	//		get the high order bit of the result... that's the "sign" bit
	//		if the sign bit not zero, replace the 0 padding with 1 padding
	return result; // replace this with the correct code
}
unsigned long getBitFldU(int bs, int len, unsigned long val)
{
	assert(bs >= 0 && bs < sizeof(long) * 8);
	assert(len > 0 && bs + len >= 0);
	unsigned long mask = ~0UL >> (64 - len);
	mask = mask << (bs - len + 1);
	// print_bits(mask);
	unsigned long result = mask & val;
	result = result >> (bs - len + 1);
	// print_bits(result);
	return result;


	// TODO:
	//		check to make sure bs is a valid bit position in long (you may assert this is true)
	//		check to make sure len is a valid length for a subfield that starts at bs (you may assert this is true)
	// 		make a mask that has len 1's
	// 		Shift that mask left so it's leftmost 1 starts at bs
	//      bitwise and the mask and val to isolate the bit field bits
	//		Shift the result to the right so that the rightmost bit of the bit field is bit 0
	//			Note that shift right pads to the left with the sign bit if the field is signed, but
	//			for unsigned, it will pad to the left with 0
	// replace this with the correct code
}

void setBitFld(int bs,int len,unsigned long new,unsigned long *val) {
	assert(bs >= 0 && bs < sizeof(long) * 8);
	assert(len > 0 && bs + len >= 0);
	unsigned long mask = ~0UL >> (64 - len);
	unsigned long result = new & mask;
	result <<= bs - len + 1;
	mask <<= bs - len + 1;
	*val = *val & ~mask;
	*val = *val | result;
}
