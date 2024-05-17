
/**
 * @file main.c
 * 
 * @copyright This file's a part of the "build" project and is distributed, and
 * licensed under "MIT" license.
 * 
 * @author joba14
 * 
 * @date 2024-05-17
 */

#include "factorial.h"

#include <stdint.h>
#include <stdio.h>

int main()
{
	for (uint64_t index = 1; index <= 10; ++index)
		(void)printf("factorial(%lu)=%lu\n", index, factorial(index));
	return 0;
}
