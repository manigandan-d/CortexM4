#include <stdio.h>
#include <stdint.h>

#define NVIC_ISER1 ((volatile uint32_t *)0xE000E104)
#define NVIC_ICER1 ((volatile uint32_t *)0XE000E184)
#define NVIC_ISPR1 ((volatile uint32_t *)0XE000E204)

#define IRQ_NO_SPI1 35
#define IRQ_NO_USART1 37

#define NVIC_PR_BASE ((volatile uint32_t *)0xE000E400)

#define NO_PR_BITS_IMPL 4

void IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority) {
    uint8_t iprx = IRQNumber / 4;
    uint8_t iprx_section = IRQNumber % 4;
    uint8_t shift_amount = (8 * iprx_section) + (8 - NO_PR_BITS_IMPL);

    *(NVIC_PR_BASE + iprx) |= (IRQPriority << shift_amount);
}

int main(void) {
	IRQPriorityConfig(IRQ_NO_SPI1, 8);
	IRQPriorityConfig(IRQ_NO_USART1, 6);
//	IRQPriorityConfig(IRQ_NO_USART1, 10);

	*NVIC_ISER1 |= (1 << (IRQ_NO_SPI1 % 32));
	*NVIC_ISER1 |= (1 << (IRQ_NO_USART1 % 32));

	*NVIC_ISPR1 |= (1 << (IRQ_NO_SPI1 % 32));

	*NVIC_ICER1 |= (1 << (IRQ_NO_SPI1 % 32));
	*NVIC_ICER1 |= (1 << (IRQ_NO_USART1 % 32));

	while(1);
}

void SPI1_IRQHandler(void) {
	printf("SPI1 ISR\n");

	*NVIC_ISPR1 |= (1 << (IRQ_NO_USART1 % 32));

	while(1);
}

void USART1_IRQHandler(void) {
	printf("USART1 ISR\n");
}
