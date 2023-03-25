#include "floatx.h"
#include <assert.h>
#include <limits.h> // for CHAR_BIT - number of bits per byte
#include <math.h> // for isinf and isnan
#include "bitFields.h"
#include <stdio.h>

union originalVal {
	double doubleVal;
	long longVal;
	float floatVal;
};
#define print_bits(x)                                    \
	do                                                   \
	{                                                    \
		unsigned long long a__ = (x);                    \
		int count = 0;									 \
		size_t bits__ = sizeof(x) * 8;                   \
		printf(#x ": ");                                 \
		while (bits__--) {								 \
			if (count % 4 == 0)printf(" ");              \
			putchar(a__ & (1ULL << bits__) ? '1' : '0'); \
			count++;									\
		}												\
		putchar('\n');                                   \
	} while (0)

floatx doubleToFloatx(double val,int totBits,int expBits) {
	assert(totBits > expBits);
	assert(expBits > 0);
	assert(0 <= totBits && totBits <= 64);
	unsigned long resultingFloat = 0UL;
	union originalVal v;
	v.doubleVal = val;
	long longVal = v.longVal;
	// print_bits(v.longVal);
	char signBit = getBit(63, longVal);
	// printf("Current signbit %d\n", signBit);
	if (signBit) setBit(totBits-1, 1, &resultingFloat);

	// Special Cases
	// 0.0
	if (v.doubleVal == 0.0) {return (float) resultingFloat;}
	// inf
	if (isinf(v.doubleVal)) {
		unsigned long allOnes = ~0UL;
		// setBit(totBits-1, 1, &resultingFloat);
		setBitFld(totBits - 2, expBits, allOnes, &resultingFloat);
		v.floatVal = resultingFloat;
		return (floatx) resultingFloat;
	}
	// NaN
	if (isnan(v.doubleVal))
	{
		long allOnes = ~0L;
		setBitFld(totBits - 2, expBits+1, allOnes, &resultingFloat);
		// print_bits(resultingFloat);
		v.floatVal = resultingFloat;
		return (floatx) resultingFloat;
	}
	// print_bits(v.longVal);
	// Getting the bias exponent
	long exp = ((unsigned long) v.longVal << 1) >>53;
	exp -= (1 << 10) - 1;

	// Do something here to check for overflow and underflow
	int minExp = 1 - (1 << (expBits - 1));
	int maxExp = (1 << (expBits-1))-1;

	if (exp > maxExp) {
		unsigned long allOnes = ~0UL;
		// setBit(totBits-1, 1, &resultingFloat);
		setBitFld(totBits - 2, expBits, allOnes, &resultingFloat);
		v.floatVal = resultingFloat;
		return (floatx)resultingFloat;
	} else if (exp < minExp) {
		return (floatx) resultingFloat;
	}
	// When exp is not overflowing or underflowing
	exp += (1 << (expBits-1))-1;
	// print_bits(exp);

	// put the bits in the resultingFloat
	setBitFld(totBits-2, expBits, exp, &resultingFloat);

	unsigned long fracbitss = (((unsigned long) ~0UL <<12) >> 12) & v.longVal;

	// doubleFracBits > longFracbits
	int longFracbits = totBits - (expBits +1);
	// printf("%d \n", longFracbits);
	if (52 > longFracbits) {
		fracbitss >>= (52 - longFracbits);
	}
	// longFracBits > doubleBits
	else if (52 < longFracbits) {
		fracbitss <<= (longFracbits - 52);
	}
	// print_bits(fracbitss);
	return fracbitss | resultingFloat;
	
		setBitFld((totBits - expBits - 2), (totBits - expBits - 1), fracbitss, &resultingFloat);
	/*-----------------------------------------------------------------------------------------------
		From the README: 	At a high level, doing the conversion requires several manipulations:
		1. Extracting the sign bit from the double v	alue, and inserting it into the floatx value at
			the correct position.
		2. Handle special cases, such as 0.0, infinity, or not-a-number.
		3. Extract the biased exponent from the double value.
				Check to see if the double value is sub-normal. If so, handle it.
				Check to make sure the floatx exponent won't overflow or underflow).
						If so, handle as a special case.
				If not, rebias the exponent using the floatx bias (which depends on the number of exponent bits)
				and write the result to the correct location in the floatx result.
		4. Extract the fraction bits from the double value.
				Determine how many bits are available for the fraction in the floatx value,
				and truncate or extend the original value,
				and write the resulting bits to the floatx result.
		6. Return the floatx result.
	----------------------------------------------------------------------------------------------------*/

	// First, make some assertions to ensure the totBits and expBits parameters are OK
	// Then, implement the algorithm

	return (floatx) resultingFloat; // Remove this when you are done.
}