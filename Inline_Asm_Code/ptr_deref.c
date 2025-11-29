#include <stdio.h>
#include <stdint.h>

int main(void) {
	uint32_t data = 10;
	uint32_t dup_data = 0;
	uint32_t *pData = &data;

	__asm volatile ("LDR %0, [%1]" : "=r"(dup_data) : "r"(pData));

	printf("Duplicate data value: %ld\n", dup_data);

	while(1);
}
