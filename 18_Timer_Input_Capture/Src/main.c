#include <stdint.h>

#define RCC_APB2ENR   (*(volatile uint32_t *)0x40021018)
#define RCC_APB1ENR   (*(volatile uint32_t *)0x4002101C)
#define GPIOA_CRL     (*(volatile uint32_t *)0x40010800)

#define TIM3_BASE     0x40000400
#define TIM3_CR1      (*(volatile uint32_t *)(TIM3_BASE + 0x00))
#define TIM3_CCMR1    (*(volatile uint32_t *)(TIM3_BASE + 0x18))
#define TIM3_CCER     (*(volatile uint32_t *)(TIM3_BASE + 0x20))
#define TIM3_SR       (*(volatile uint32_t *)(TIM3_BASE + 0x10))
#define TIM3_PSC      (*(volatile uint32_t *)(TIM3_BASE + 0x28))
#define TIM3_ARR      (*(volatile uint32_t *)(TIM3_BASE + 0x2C))
#define TIM3_CCR1     (*(volatile uint32_t *)(TIM3_BASE + 0x34))

void tim3_ic_init(void) {
    RCC_APB2ENR |= (1 << 2);  // GPIOA
    RCC_APB1ENR |= (1 << 1);  // TIM3

    // PA6: Input Floating (TIM3_CH1)
    GPIOA_CRL &= ~(0xF << 24);
    GPIOA_CRL |= (0x4 << 24);

    TIM3_PSC = 7;             // 8MHz / 8 = 1MHz (1us tick)
    TIM3_ARR = 0xFFFF;

    // CC1S = 01 (Channel 1 as Input), No Filter
    TIM3_CCMR1 |= (1 << 0);

    // CC1E = 1 (Enable Capture), CC1P = 0 (Rising Edge)
    TIM3_CCER |= (1 << 0);

    TIM3_CR1 |= (1 << 0);     // Start
}

uint32_t get_capture_value(void) {
    while (!(TIM3_SR & (1 << 1))); // Wait for CC1IF
    return TIM3_CCR1;
}

int main(void) {
    uint32_t last_val = 0;
    uint32_t current_val = 0;
    uint32_t diff = 0;

    tim3_ic_init();

    while (1) {
        current_val = get_capture_value();

        if (current_val > last_val) {
            diff = current_val - last_val;
        } else {
            diff = (0xFFFF - last_val) + current_val;
        }

        last_val = current_val;

        // Frekans Hesabı: 1.000.000 / diff (Hz)
    }
}
