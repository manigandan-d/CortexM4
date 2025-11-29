#include <stdio.h>
#include <stdint.h>

int main(void) {
	uint32_t data = 10;

	__asm volatile ("MOV R0, %0" :: "r"(data));

//	__asm volatile ("LDR R1, =#0xAB");
	__asm volatile ("MOV R1, %0" :: "i"(0xAB));

	while(1);
}
