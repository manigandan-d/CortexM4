#include <stdio.h>
#include <stdint.h>

void check_processor_mode(void) {
	uint32_t ipsr_value;

	__asm volatile ("MRS %0, IPSR" : "=r" (ipsr_value));

	if(ipsr_value == 0) {
		printf("\tProcessor is in Thread mode\n");
	}

	else {
		printf("\tProcessor is in Handler mode -> ISR_NUMBER: %ld\n", ipsr_value);

		if(ipsr_value <= 15) {
			printf("\t\tCurrently in System Exception Handler\n");
		}

		else {
			printf("\t\tCurrently in IRQ%ld Handler\n", ipsr_value - 16);
		}
	}
}

void generate_interrupt(void) {
	uint32_t *pISER0 = (uint32_t *)0xE000E100;
	uint32_t *pSTIR = (uint32_t *)0xE000EF00;

	*pISER0 |= (1 << 3);

	*pSTIR = (3 & 0x1FF);
}

int main(void) {
	printf("In thread mode: Checking processor mode...\n");
	check_processor_mode();

	generate_interrupt();

	while(1);
}

void RTC_WKUP_IRQHandler(void) {
	printf("In handler mode: Checking processor mode...\n");
	check_processor_mode();
}
