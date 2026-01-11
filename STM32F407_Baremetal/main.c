#include "main.h"
#include "led.h"

int main(void) {
	enable_processor_faults();

	initialise_monitor_handles();

	init_sched_stack(SCHED_STACK_START);

	init_tasks_stack();

	led_init_all();

	init_systick_timer(TICK_HZ);

	switch_sp_to_psp();

	printf("Program is running\n");

	task1_handler();

	while(1);
}

void idle_task(void) {
	while(1);
}

void task1_handler(void) {
	while(1) {
		printf("Task_1\n");

		led_on(LED_GREEN);
		task_delay(1000);
		led_off(LED_GREEN);
		task_delay(1000);
	}
}

void task2_handler(void) {
	while(1) {
		printf("Task_2\n");

		led_on(LED_ORANGE);
		task_delay(500);
		led_off(LED_ORANGE);
		task_delay(500);
	}
}

void task3_handler(void) {
	while(1) {
		printf("Task_3\n");

		led_on(LED_BLUE);
		task_delay(250);
		led_off(LED_BLUE);
		task_delay(250);
	}
}

void task4_handler(void) {
	while(1) {
		printf("Task_4\n");

		led_on(LED_RED);
		task_delay(125);
		led_off(LED_RED);
		task_delay(125);
	}
}

void init_systick_timer(uint32_t tick_hz) {
	uint32_t *pSCSR = (uint32_t *)0xE000E010;
	uint32_t *pSRVR = (uint32_t *)0xE000E014;

	uint32_t count = (SYSTICK_TIM_CLK / tick_hz) - 1;

	*pSRVR &= ~(0x00FFFFFF);
	*pSRVR |= count;

	*pSCSR |= (1 << 2);
	*pSCSR |= (1 << 1);
	*pSCSR |= (1 << 0);
}

__attribute__((naked)) void init_sched_stack(uint32_t sched_stack_top) {
	__asm volatile ("MSR MSP, %0" :: "r"(sched_stack_top));
	__asm volatile ("BX LR");
}

void init_tasks_stack(void) {
	for(int i=0; i<MAX_TASK; i++) {
		user_tasks[i].state = TASK_READY;
		user_tasks[i].block_count = 0;
	}

	user_tasks[0].psp = IDLE_STACK_START;
	user_tasks[1].psp = T1_STACK_START;
	user_tasks[2].psp = T2_STACK_START;
	user_tasks[3].psp = T3_STACK_START;
	user_tasks[4].psp = T4_STACK_START;

	user_tasks[0].task_handler = idle_task;
	user_tasks[1].task_handler = task1_handler;
	user_tasks[2].task_handler = task2_handler;
	user_tasks[3].task_handler = task3_handler;
	user_tasks[4].task_handler = task4_handler;

	for(int i=0; i<MAX_TASK; i++) {
		uint32_t *psp = (uint32_t *)user_tasks[i].psp;

		psp--;
		*psp = DUMMY_XPSR;  // xPSR

		psp--;
		*psp = ((uint32_t)user_tasks[i].task_handler | 1U);  // PC

		psp--;
		*psp = EXC_RETURN_PSP;  // LR

		psp--;
		*psp = 0;  // R12

		psp--;
		*psp = 0;  // R3

		psp--;
		*psp = 0;  // R2

		psp--;
		*psp = 0;  // R1

		psp--;
		*psp = 0;  // R0

		for(int reg=0; reg<SCHED_FRAME_WORDS; reg++) {
			psp--;
			*psp = 0;  // R4-R11
		}

		user_tasks[i].psp = (uint32_t)psp;
	}
}

void enable_processor_faults(void) {
	uint32_t *pSHCSR = (uint32_t *)0xE000ED24;

	*pSHCSR |= (1 << 16);
	*pSHCSR |= (1 << 17);
	*pSHCSR |= (1 << 18);
}

void HardFault_Handler(void) {
	printf("HardFault occurred\n");
	while(1);
}

void MemManage_Handler(void) {
	printf("MemManage Fault occurred\n");
	while(1);
}

void BusFault_Handler(void) {
	printf("BusFault occurred\n");
	while(1);
}

void UsageFault_Handler(void) {
	printf("UsageFault occurred\n");
	while(1);
}

uint32_t get_psp_value(void) {
	return user_tasks[curr_task].psp;
}

void set_psp_value(uint32_t val) {
	user_tasks[curr_task].psp = val;
}

__attribute__((naked)) void switch_sp_to_psp(void) {
	__asm volatile ("PUSH {LR}");
	__asm volatile ("BL get_psp_value");
	__asm volatile ("MSR PSP, R0");
	__asm volatile ("POP {LR}");

	__asm volatile ("MRS R0, CONTROL");
	__asm volatile ("ORR R0, R0, #2");
	__asm volatile ("MSR CONTROL, R0");

	__asm volatile ("BX LR");
}

void update_g_tick_count(void) {
	g_tick_count++;
}

void unblock_tasks(void) {
	for(int i=1; i<MAX_TASK; i++) {
		if(user_tasks[i].state == TASK_BLOCKED) {
			if(g_tick_count >= user_tasks[i].block_count) {
				user_tasks[i].state = TASK_READY;
			}
		}
	}
}

void SysTick_Handler(void) {
	uint32_t *pICSR = (uint32_t *)0xE000ED04;

	update_g_tick_count();
	unblock_tasks();

	*pICSR |= (1 << 28);
}

void schedule(void) {
	uint32_t *pICSR = (uint32_t *)0xE000ED04;
	*pICSR |= (1 << 28);
}

void task_delay(uint32_t tick_count) {
	INTR_DISABLE();

	if(curr_task) {
		user_tasks[curr_task].block_count = g_tick_count + tick_count;
		user_tasks[curr_task].state = TASK_BLOCKED;

		schedule();
	}

	INTR_ENABLE();
}

void update_next_task(void) {
	uint8_t state = TASK_BLOCKED;

	for(int i=0; i<MAX_TASK; i++) {
		curr_task++;
		curr_task %= MAX_TASK;

		state = user_tasks[curr_task].state;

		if((state == TASK_READY) && (curr_task != 0))
			break;
	}

	if(state != TASK_READY)
		curr_task = 0;
}

__attribute__((naked)) void PendSV_Handler(void) {
	__asm volatile ("MRS R0, PSP");
	__asm volatile ("STMDB R0!, {R4-R11}");
	__asm volatile ("PUSH {LR}");
	__asm volatile ("BL set_psp_value");

	__asm volatile ("BL update_next_task");
	__asm volatile ("BL get_psp_value");
	__asm volatile ("LDMIA R0!, {R4-R11}");
	__asm volatile ("MSR PSP, R0");
	__asm volatile ("POP {LR}");

	__asm volatile ("BX LR");
}
