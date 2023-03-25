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
floatx doubleToFloatx(double val,int totBits,int expBits) {
	assert(totBits > expBits);
	assert(expBits > 0);
	assert(0 <= totBits && totBits <= 64);
	unsigned long resultingFloat = 0UL;
	union originalVal v;
	v.doubleVal = val;
	long longVal = v.longVal;

	// Setting the sign bit
	char signBit = getBit(63, longVal);
	if (signBit) setBit(totBits-1, 1, &resultingFloat);

	// Special Cases
	// 0.0
	if (v.doubleVal == 0.0) {return (float) resultingFloat;}
	// inf
	if (isinf(v.doubleVal)) {
		unsigned long allOnes = ~0UL;
		setBitFld(totBits - 2, expBits, allOnes, &resultingFloat);
		v.floatVal = resultingFloat;
		return (floatx) resultingFloat;
	}
	// NaN
	if (isnan(v.doubleVal))
	{
		long allOnes = ~0L;
		setBitFld(totBits - 2, expBits+1, allOnes, &resultingFloat);
		v.floatVal = resultingFloat;
		return (floatx) resultingFloat;
	}

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

	// put the new exp bits in the resultingFloat
	setBitFld(totBits-2, expBits, exp, &resultingFloat);

	// Get the fraction bits
	unsigned long fracbits = (((unsigned long) ~0UL <<12) >> 12) & v.longVal;

	// doubleFracBits > longFracbits -- remove bits
	int longFracbits = totBits - (expBits +1);
	// printf("%d \n", longFracbits);
	if (52 > longFracbits) {
		fracbits >>= (52 - longFracbits);
	}
	// longFracBits > doubleBits -- add bits
	else if (52 < longFracbits) {
		fracbits <<= (longFracbits - 52);
	}

	return fracbits | resultingFloat;
	
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
}