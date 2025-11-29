#include <stdio.h>
#include <stdint.h>

int main(void) {
	uint32_t *SCB_AIRCR = (uint32_t *)0xE000ED0C;

	uint32_t endianness = (*SCB_AIRCR >> 15) & 0x1;

	printf("Endiannes value: %ld\n", endianness);

    if (endianness == 0) {
        printf("Processor is Little-endian\n");
    }

    else {
        printf("Processor is Big-endian\n");
    }

	while(1);
}
