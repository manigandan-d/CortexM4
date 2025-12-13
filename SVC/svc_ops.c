#include <stdio.h>
#include <stdint.h>

int32_t add(int32_t x, int32_t y) {
	int32_t res;

	__asm volatile ("SVC #1");
	__asm volatile ("MOV %0, R0" : "=r"(res));

	return res;
}

int32_t sub(int32_t x, int32_t y) {
	int32_t res;

	__asm volatile ("SVC #2");
	__asm volatile ("MOV %0, R0" : "=r"(res));

	return res;
}

int32_t mul(int32_t x, int32_t y) {
	int32_t res;

	__asm volatile ("SVC #3");
	__asm volatile ("MOV %0, R0" : "=r"(res));

	return res;
}

int32_t div(int32_t x, int32_t y) {
	int32_t res;

	__asm volatile ("SVC #4");
	__asm volatile ("MOV %0, R0" : "=r"(res));

	return res;
}

int main(void) {
	int32_t a = 10, b = 5, res = 0;

	res = add(a, b);
	printf("Addition result: %ld\n", res);

	res = sub(a, b);
	printf("Subtraction result: %ld\n", res);

	res = mul(a, b);
	printf("Multiplication result: %ld\n", res);

	res = div(a, b);
	printf("Division result: %ld\n", res);

	while(1);
}

__attribute__((naked)) void SVC_Handler(void) {
	__asm volatile ("MRS R0, MSP");
	__asm volatile ("B SVC_Handler_C");
}

void SVC_Handler_C(uint32_t *pStackFrame) {
	uint8_t *pc = (uint8_t *)pStackFrame[6];

	pc -= 2;

	uint8_t svc_num = *pc;

	int32_t arg0 = pStackFrame[0];
	int32_t arg1 = pStackFrame[1];
	int32_t res;

	switch(svc_num) {
		case 1:
			res = arg0 + arg1;
			break;
		case 2:
			res = arg0 - arg1;
			break;
		case 3:
			res = arg0 * arg1;
			break;
		case 4:
			res = arg0 / arg1;
			break;
		default:
			printf("Invalid SVC Number: %d\n", svc_num);
	}

	pStackFrame[0] = res;
}
