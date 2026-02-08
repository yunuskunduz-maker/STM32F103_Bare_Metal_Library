#include <stdint.h>

#define RCC_APB2ENR   (*(volatile uint32_t *)0x40021018)
#define GPIOA_CRL     (*(volatile uint32_t *)0x40010800)
#define GPIOA_ODR     (*(volatile uint32_t *)0x4001080C)
#define GPIOC_CRH     (*(volatile uint32_t *)0x40011004)
#define AFIO_EXTICR4  (*(volatile uint32_t *)0x40010014)
#define EXTI_IMR      (*(volatile uint32_t *)0x40010400)
#define EXTI_FTSR     (*(volatile uint32_t *)0x40010408)
#define EXTI_PR       (*(volatile uint32_t *)0x40010414)
#define NVIC_ISER1    (*(volatile uint32_t *)0xE000E104)
#define SCB_SCR       (*(volatile uint32_t *)0xE000ED10) // System Control Register

void system_init(void) {
    RCC_APB2ENR |= (1 << 0) | (1 << 2) | (1 << 4);

    // PA5: Output (LED)
    GPIOA_CRL &= ~(0xF << 20);
    GPIOA_CRL |= (0x2 << 20);

    // PC13: Input Floating (Button)
    GPIOC_CRH &= ~(0xF << 20);
    GPIOC_CRH |= (0x4 << 20);

    // EXTI13 configuration
    AFIO_EXTICR4 &= ~(0xF << 4);
    AFIO_EXTICR4 |= (0x2 << 4);
    EXTI_IMR |= (1 << 13);
    EXTI_FTSR |= (1 << 13);
    NVIC_ISER1 |= (1 << 8);
}

void EXTI15_10_IRQHandler(void) {
    if (EXTI_PR & (1 << 13)) {
        GPIOA_ODR |= (1 << 5); // Wake up indication
        EXTI_PR |= (1 << 13);
    }
}

int main(void) {
    system_init();

    while (1) {
        for (volatile int i = 0; i < 2000000; i++); // Wait before sleep
        GPIOA_ODR &= ~(1 << 5); // LED off before sleep

        // Sleep Mode Configuration
        SCB_SCR &= ~(1 << 2); // SLEEPDEEP = 0 (Sleep mode)

        __asm volatile ("wfi"); // Wait For Interrupt instruction

        // Code resumes here after interrupt
        for (volatile int i = 0; i < 1000000; i++);
    }
}
