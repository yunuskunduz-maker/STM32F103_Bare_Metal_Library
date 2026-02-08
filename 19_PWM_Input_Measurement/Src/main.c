#include <stdint.h>

#define RCC_APB2ENR   (*(volatile uint32_t *)0x40021018)
#define RCC_APB1ENR   (*(volatile uint32_t *)0x4002101C)
#define GPIOA_CRL     (*(volatile uint32_t *)0x40010800)

#define TIM3_BASE     0x40000400
#define TIM3_CR1      (*(volatile uint32_t *)(TIM3_BASE + 0x00))
#define TIM3_SMCR     (*(volatile uint32_t *)(TIM3_BASE + 0x08))
#define TIM3_SR       (*(volatile uint32_t *)(TIM3_BASE + 0x10))
#define TIM3_CCMR1    (*(volatile uint32_t *)(TIM3_BASE + 0x18))
#define TIM3_CCER     (*(volatile uint32_t *)(TIM3_BASE + 0x20))
#define TIM3_PSC      (*(volatile uint32_t *)(TIM3_BASE + 0x28))
#define TIM3_ARR      (*(volatile uint32_t *)(TIM3_BASE + 0x2C))
#define TIM3_CCR1     (*(volatile uint32_t *)(TIM3_BASE + 0x34))
#define TIM3_CCR2     (*(volatile uint32_t *)(TIM3_BASE + 0x38))

void tim3_pwmi_init(void) {
    RCC_APB2ENR |= (1 << 2);
    RCC_APB1ENR |= (1 << 1);

    // PA6: Input Floating (TIM3_CH1)
    GPIOA_CRL &= ~(0xF << 24);
    GPIOA_CRL |= (0x4 << 24);

    TIM3_PSC = 7; // 1MHz clock
    TIM3_ARR = 0xFFFF;

    // CH1: Map to IC1, CH2: Map to IC1
    TIM3_CCMR1 |= (1 << 0) | (2 << 8);

    // CC1P: Rising, CC2P: Falling
    TIM3_CCER |= (1 << 0) | (1 << 4) | (1 << 5);

    // Slave Mode: Reset on TI1FP1
    TIM3_SMCR |= (5 << 4) | (4 << 0);

    TIM3_CR1 |= (1 << 0);
}

int main(void) {
    uint32_t period = 0;
    uint32_t duty_width = 0;
    float duty_cycle = 0;

    tim3_pwmi_init();

    while (1) {
        if (TIM3_SR & (1 << 1)) {
            period = TIM3_CCR1;
            duty_width = TIM3_CCR2;

            if (period > 0) {
                duty_cycle = ((float)duty_width / (float)period) * 100.0f;
            }
        }
    }
}
