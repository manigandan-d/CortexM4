#include <stdint.h>
#include "led.h"

void led_init_all(void) {
	uint32_t *pRccAhb1enr = (uint32_t*)0x40023830;
	uint32_t *pGpiodModeReg = (uint32_t*)0x40020C00;
	uint32_t *pGpioOpTypeReg = (uint32_t *)0x40020C04;

	*pRccAhb1enr |= ( 1 << 3);

	*pGpiodModeReg |= (1 << (2 * LED_GREEN));
	*pGpiodModeReg |= (1 << (2 * LED_ORANGE));
	*pGpiodModeReg |= (1 << (2 * LED_RED));
	*pGpiodModeReg |= (1 << (2 * LED_BLUE));

	*pGpioOpTypeReg |= (1 << (2 * LED_GREEN));
	*pGpioOpTypeReg |= (1 << (2 * LED_ORANGE));
	*pGpioOpTypeReg |= (1 << (2 * LED_RED));
	*pGpioOpTypeReg |= (1 << (2 * LED_BLUE));

    led_off(LED_GREEN);
    led_off(LED_ORANGE);
    led_off(LED_RED);
    led_off(LED_BLUE);
}

void led_on(uint8_t led_no) {
	uint32_t *pGpiodDataReg = (uint32_t*)0x40020C14;
	*pGpiodDataReg |= (1 << led_no);
}

void led_off(uint8_t led_no) {
	uint32_t *pGpiodDataReg = (uint32_t*)0x40020C14;
	*pGpiodDataReg &= ~(1 << led_no);

}
