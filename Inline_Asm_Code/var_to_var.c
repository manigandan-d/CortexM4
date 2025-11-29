#include <stdio.h>
#include <stdint.h>

int main(void) {
	uint32_t data = 10;
	uint32_t dup_data = 0;

	__asm volatile ("MOV %0, %1" : "=r"(dup_data) : "r"(data));

	printf("Duplicate data value: %ld\n", dup_data);

	while(1);
}
