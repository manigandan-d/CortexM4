#include <stdint.h>
#include <stdio.h>

#define SRAM_START 		0x20000000UL
#define SRAM_SIZE 		(128 * 1024)
#define SRAM_END  		((SRAM_START) + (SRAM_SIZE))

#define TOTAL_STACK_SIZE 	1024U
#define MSP_SIZE 			(TOTAL_STACK_SIZE / 2)
#define PSP_SIZE 			(TOTAL_STACK_SIZE / 2)

#define MSP_START	SRAM_END
#define MSP_END 	((MSP_START) - (MSP_SIZE))
#define PSP_START	MSP_END
#define PSP_END 	((PSP_START) - (PSP_SIZE))

void check_sp_mode(void) {
	uint32_t control_reg;

	__asm volatile ("MRS %0, CONTROL" : "=r"(control_reg));

	if(control_reg & (1<<1)) {
		printf("SP: PSP (Thread mode)\n");
	}
	else {
		printf("SP: MSP (Handler mode or default thread mode)\n");
	}
}

__attribute__((naked)) void switch_to_psp(void) {
	__asm volatile ("MOV R0, %0" :: "r"(PSP_START));
	__asm volatile ("MSR PSP, R0");

	__asm volatile (
		"MRS R1, CONTROL\n\t"
		"ORR R1, R1, #2\n\t"
		"MSR CONTROL, R1\n\t"
	);

	__asm volatile ("BX LR");
}

int main(void) {
	check_sp_mode();

	switch_to_psp();
	check_sp_mode();

	__asm volatile ("SVC #0");

    check_sp_mode();

    while(1);
}

void SVC_Handler(void) {
	check_sp_mode();
}
