#include <stdint.h>
#include <string.h>

#include "ieee754_clf.h"

float_class_t classify(double x) {
	int binary[64];
	union {
		double p1;
		uint64_t p2;
	} tmp;
	tmp.p1 = x;
	int mantissa = 0;
	int exponent = 0;
	int sign;
	int pos = 0;
	
	for (uint64_t i = 1ULL<<63; i > 0; i = i>>1) {
		if((tmp.p2 & i) != 0) {
			binary[pos] = 1;
		} else {
			binary[pos] = 0;
		}
		pos += 1;
	}

	sign = binary[0];
	for (int i = 1; i < 12; ++i) {
		exponent += binary[i];
	}
	for (int i = 12; i < 64; ++i) {
		mantissa += binary[i];
	}

	if(exponent == 11 && mantissa == 0) {
		if (sign) {
			return MinusInf;
		}
		return Inf;
	} else if (exponent > 0 && exponent < 11) {
		if (sign) {
			return MinusRegular;
		}
		return Regular;
	} else if (exponent == 0 && mantissa != 0) {
		if(sign) {
			return MinusDenormal;
		}
		return Denormal;
	} else if (exponent == 11 && mantissa != 0) {
		return NaN;
	} else if (exponent == 0 && mantissa == 0 && sign) {
		return MinusZero;
	}
  return Zero;
}
