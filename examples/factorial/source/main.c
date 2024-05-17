
#include "factorial.h"

#include <stdint.h>
#include <stdio.h>

int main()
{
	for (uint64_t index = 1; index <= 10; ++index)
		(void)printf("factorial(%lu)=%lu\n", index, factorial(index));
	return 0;
}
