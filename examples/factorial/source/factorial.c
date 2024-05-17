
/**
 * @file factorial.c
 * 
 * @copyright This file's a part of the "build" project and is distributed, and
 * licensed under "MIT" license.
 * 
 * @author joba14
 * 
 * @date 2024-05-17
 */

#include "factorial.h"

uint64_t factorial(const uint64_t value)
{
	if (value <= 1) return 1;
	return value * factorial(value - 1);
}
