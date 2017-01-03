#ifndef __SOFT_UDIVIDE_H__
#define __SOFT_UDIVIDE_H__


typedef unsigned long UINT_TYPE;
#define INTL (sizeof(UINT_TYPE) * 8)

UINT_TYPE soft_udivide(UINT_TYPE dividend, UINT_TYPE divisor);
UINT_TYPE soft_umod(UINT_TYPE dividend, UINT_TYPE divisor);
void soft_udiv(
	UINT_TYPE *dividend,
	UINT_TYPE *divisor,
	UINT_TYPE *quotient
	);

#endif
