#include <stdint.h>

// Adres Tanımlamaları
#define RCC_BASE      0x40021000
#define RCC_APB2ENR   (*(volatile uint32_t *)(RCC_BASE + 0x18))
#define GPIOA_BASE    0x40010800
#define GPIOA_CRL     (*(volatile uint32_t *)(GPIOA_BASE + 0x00))
#define GPIOA_ODR     (*(volatile uint32_t *)(GPIOA_BASE + 0x0C))

void delay(uint32_t count) {
    for (uint32_t i = 0; i < count; i++) __asm("nop");
}

int main(void) {
    // 1. GPIOA Saat Sinyalini Aç
    RCC_APB2ENR |= (1 << 2);

    // 2. PA5 Pinini Output Mode (2MHz) Yap
    GPIOA_CRL &= ~(0xF << 20);
    GPIOA_CRL |= (0x2 << 20);

    while (1) {
        GPIOA_ODR ^= (1 << 5); // XOR ile durumu tersine çevir (Toggle)
        delay(500000);
    }
}
