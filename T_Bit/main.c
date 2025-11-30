#include <stdio.h>
#include <stdint.h>

void display(void) {
	printf("Hello from display function\n");
}

int main(void) {
//	void (*fptr)(void) = display;

	void (*fptr)(void) = (void *)((uint32_t)display & ~1U);

	fptr();

    while(1);
}

void HardFault_Handler(void) {
	printf("Hard fault detected\n");

	while(1);
}
