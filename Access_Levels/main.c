#include <stdio.h>
#include <stdint.h>

void check_access_level() {
	uint32_t control_reg;

	__asm volatile ("MRS %0, CONTROL" : "=r" (control_reg));

	if(control_reg == 0) {
		printf("\tRunning in Privileged Access Level\n");
	}
	else {
		printf("\tRunning in Unprivileged Access Level\n");
	}
}

int main(void) {
	printf("In thread mode: Checking access level...\n");
	check_access_level();

	__asm volatile (
		"MRS R0, CONTROL\n\t"
		"ORR R0, R0, #1\n\t"
		"MSR CONTROL, R0\n\t"
	);

	__asm volatile ("SVC #0");

	printf("After SVC instruction: Checking access level...\n");
	check_access_level();

	while(1);
}

void SVC_Handler(void) {
	printf("In handler mode: Changing access level...\n");

	__asm volatile (
		"MRS R0, CONTROL\n\t"
		"BIC R0, R0, #1\n\t"
		"MSR CONTROL, R0\n\t"
	);
}
