#include <stdio.h>
#include <stdint.h>

int main(void) {
	uint32_t control_reg;

	__asm volatile ("MRS %0, CONTROL" : "=r"(control_reg));

	printf("Control Register value: %ld\n", control_reg);

	while(1);
}
