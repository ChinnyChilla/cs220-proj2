#include "floatx.h"
#include <assert.h>
#include <limits.h> // for CHAR_BIT - number of bits per byte
#include <math.h> // for isinf and isnan
#include "bitFields.h"
#include <stdio.h>

#define print_bits(x)                                    \
	do                                                   \
	{                                                    \
		unsigned long long a__ = (x);                    \
		size_t bits__ = sizeof(x) * 8;                   \
		printf(#x ": ");                                 \
		while (bits__--)                                 \
			putchar(a__ & (1ULL << bits__) ? '1' : '0'); \
		putchar('\n');                                   \
	} while (0)

floatx doubleToFloatx(double val,int totBits,int expBits) {
	assert(totBits > expBits);
	assert(expBits > 0);
	assert(0 <= totBits && totBits <= 64);
	unsigned long resultingFloat = 0UL;
	print_bits(val);
	char signBit = getBit(63, val);
	printf("Current signbit %d\n", signBit);
	if (signBit) setBit(totBits-1, 1, &resultingFloat);

	// Special Cases
	// 0.0
	if (val == 0.0) {return (float) resultingFloat;}
	// inf
	if (isinf(val)) {
		unsigned long allOnes = ~0UL;
		setBit(totBits-1, 1, &resultingFloat);
		setBitFld(totBits - 2, expBits, allOnes, &resultingFloat);
		return (float) resultingFloat;
	}
	// NaN
	if (isnan(val))
	{
		unsigned long allOnes = ~0UL;
		setBitFld(totBits - 2, expBits, allOnes, &resultingFloat);
		setBit(0, 1, &resultingFloat);
		return (float)resultingFloat;
	}
		/*-----------------------------------------------------------------------------------------------
			From the README: 	At a high level, doing the conversion requires several manipulations:
			1. Extracting the sign bit from the double value, and inserting it into the floatx value at
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

		return (float) resultingFloat; // Remove this when you are done.
}