#include <stdio.h>
#include <stdint.h>

int main(void) {
	__asm volatile ("LDR R0, =#0x20001000");
	__asm volatile ("LDR R1, =#0x20001004");

	__asm volatile ("LDR R2, [R0]");
	__asm volatile ("LDR R3, [R1]");

	__asm volatile ("ADD R2, R2, R3");

	__asm volatile ("STR R2, [R0]");

	while(1);
}
