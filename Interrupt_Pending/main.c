#include <stdio.h>
#include <stdint.h>

#define NVIC_ISER1 ((volatile uint32_t *)0xE000E104)
#define NVIC_ICER1 ((volatile uint32_t *)0XE000E184)
#define NVIC_ISPR1 ((volatile uint32_t *)0XE000E204)

#define IRQ_NO_USART1 37
#define IRQ_NO_USART2 38
#define IRQ_NO_USART3 39

int main(void) {
	*NVIC_ISPR1 |= (1 << (IRQ_NO_USART1 % 32));
	*NVIC_ISER1 |= (1 << (IRQ_NO_USART1 % 32));
	*NVIC_ICER1 |= (1 << (IRQ_NO_USART1 % 32));

	*NVIC_ISPR1 |= (1 << (IRQ_NO_USART2 % 32));
	*NVIC_ISER1 |= (1 << (IRQ_NO_USART2 % 32));
	*NVIC_ICER1 |= (1 << (IRQ_NO_USART2 % 32));

	*NVIC_ISPR1 |= (1 << (IRQ_NO_USART3 % 32));
	*NVIC_ISER1 |= (1 << (IRQ_NO_USART3 % 32));
	*NVIC_ICER1 |= (1 << (IRQ_NO_USART3 % 32));
}

void USART1_IRQHandler(void) {
	printf("USART1 ISR is executing...\n");
}

void USART2_IRQHandler(void) {
	printf("USART2 ISR is executing...\n");
}

void USART3_IRQHandler(void) {
	printf("USART3 ISR is executing...\n");
}
