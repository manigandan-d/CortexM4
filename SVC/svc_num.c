#include <stdio.h>
#include <stdint.h>

void SVC_Handler_C(uint32_t *);

int main(void) {
	__asm volatile ("SVC #25");

	uint32_t data;
	__asm volatile ("MOV %0, R0" : "=r"(data));
	printf("Data received from SVC handler: %ld\n", data);

	while(1);
}

__attribute__((naked)) void SVC_Handler(void) {
	__asm volatile ("MRS R0, MSP");
	__asm volatile ("B SVC_Handler_C");
}

void SVC_Handler_C(uint32_t *pStackFrame) {
	uint8_t *pc = (uint8_t *)pStackFrame[6];

	pc -= 2;

	uint8_t svc_number = *pc;
	printf("SVC Number: %d\n", svc_number);

	svc_number += 5;

	pStackFrame[0] = svc_number;
}
