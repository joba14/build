
#include "factorial.h"

uint64_t factorial(const uint64_t value)
{
	if (value <= 1) return 1;
	return value * factorial(value - 1);
}
