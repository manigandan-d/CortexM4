#ifndef MAIN_H_
#define MAIN_H_

#include <stdio.h>
#include <stdint.h>

#define MAX_TASK 			5

#define TASK_STACK_SIZE  	1024U
#define SCHED_STACK_SIZE 	1024U

#define SRAM_START 			0x20000000U
#define SRAM_SIZE  			((128) * (1024))
#define SRAM_END   			((SRAM_START) + (SRAM_SIZE))

#define T1_STACK_START   	SRAM_END
#define T2_STACK_START   	((SRAM_END) - (1 * TASK_STACK_SIZE))
#define T3_STACK_START 	 	((SRAM_END) - (2 * TASK_STACK_SIZE))
#define T4_STACK_START 	 	((SRAM_END) - (3 * TASK_STACK_SIZE))
#define IDLE_STACK_START 	((SRAM_END) - (4 * TASK_STACK_SIZE))

#define SCHED_STACK_START 	((SRAM_END) - (5 * TASK_STACK_SIZE))

#define TICK_HZ  			1000U
#define HSI_CLK  			16000000U
#define SYSTICK_TIM_CLK 	HSI_CLK

#define TASK_READY 			0x00
#define TASK_BLOCKED 		0xFF

#define DUMMY_XPSR 			0x01000000U
#define EXC_RETURN_PSP 	 	0xFFFFFFFD

#define CPU_FRAME_WORDS 	8
#define SCHED_FRAME_WORDS	8

#define INTR_ENABLE() 		do{__asm volatile ("MOV R0, #0x0"); __asm volatile ("MSR PRIMASK, R0");}while(0)
#define INTR_DISABLE() 		do{__asm volatile ("MOV R0, #0x1"); __asm volatile ("MSR PRIMASK, R0");}while(0);

typedef struct {
	uint32_t psp;
	uint32_t block_count;
	uint8_t state;
	void (*task_handler)(void);
}TCB_t;

TCB_t user_tasks[MAX_TASK];

uint8_t curr_task = 1;
uint32_t g_tick_count = 0;

void initialise_monitor_handles(void);

void idle_task(void);
void task1_handler(void);
void task2_handler(void);
void task3_handler(void);
void task4_handler(void);

void init_systick_timer(uint32_t tick_hz);
__attribute__((naked)) void init_sched_stack(uint32_t sched_stack_top);
void init_tasks_stack(void);

void enable_processor_faults(void);

uint32_t get_psp_value(void);
void set_psp_value(uint32_t val);
__attribute__((naked)) void switch_sp_to_psp(void);

void unblock_tasks(void);
void update_g_tick_count(void);

void schedule(void);
void task_delay(uint32_t tick_count);

void update_next_task(void);

#endif /* MAIN_H_ */
