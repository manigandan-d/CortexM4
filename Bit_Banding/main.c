#include <stdint.h>
#include <stdio.h>

#define SRAM_BASE 			0x20000000
#define BITBAND_SRAM_BASE 	0x22000000

int main(void) {
    volatile uint32_t *addr = (uint32_t *)0x20001000;

    // Bit-band formula: alias_addr = bitband_base + (byte_offset * 32) + (bit_number * 4)
    volatile uint32_t *alias_addr = (uint32_t *)(BITBAND_SRAM_BASE + (((uint32_t)addr-SRAM_BASE) * 32) + (3 * 4));

    *addr = 0xFF;

    *alias_addr = 0;

    printf("Value at original address: 0x%08lX\n", *addr);
    printf("Alias address: 0x%08lX\n", (uint32_t)alias_addr);
    printf("Value at alias address: %ld\n", *alias_addr);

    while(1);
}
