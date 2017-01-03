#include "soft_udivide.h"

void soft_udiv(
	UINT_TYPE *dividend,
	UINT_TYPE *divisor,
	UINT_TYPE *quotient) {
	if (*divisor == 0) return;
	*quotient = 0;
	if (*dividend == 0) return;
	if ((1L << (INTL - 1) & *divisor)) {
		if (*dividend >= *divisor) {
			*quotient = 1;
			*dividend -= *divisor;
		}
		return;
	}

	char l1 = 0;
	char l2 = 0;

	while (!(1L << (INTL - 2) & *dividend) ) {
		l1++;
		*dividend <<= 1;
	}
	while ( !(1L << (INTL - 2) & *divisor) ) {
		l2++;
		*divisor <<= 1;
	}
	l1 = INTL - l1 - 1;
	l2 = INTL - l2 - 1;

	while ( l1 >= l2 ) {
		if (*dividend >= *divisor) {
			*quotient |= 1;
			*dividend -= *divisor;
		}
		l1--;
		*dividend <<= 1;
		*quotient <<= 1;
	}
	*dividend >>= INTL - l1 - 1;
	*quotient >>= 1;
}

UINT_TYPE soft_udivide(UINT_TYPE dividend, UINT_TYPE divisor) {
	UINT_TYPE quotient;
	soft_udiv(&dividend, &divisor, &quotient);
	return quotient;
}

UINT_TYPE soft_umod(UINT_TYPE dividend, UINT_TYPE divisor) {
	UINT_TYPE quotient;
	soft_udiv(&dividend, &divisor, &quotient);
	return dividend;
}
