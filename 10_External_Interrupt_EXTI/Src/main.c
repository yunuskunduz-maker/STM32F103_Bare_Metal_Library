#include <stdint.h>

#define RCC_APB2ENR   (*(volatile uint32_t *)(0x40021018))
#define GPIOA_CRL     (*(volatile uint32_t *)(0x40010800))
#define GPIOA_ODR     (*(volatile uint32_t *)(0x4001080C))
#define GPIOC_CRH     (*(volatile uint32_t *)(0x40011004))
#define AFIO_EXTICR4  (*(volatile uint32_t *)(0x40010014))
#define EXTI_IMR      (*(volatile uint32_t *)(0x40010400))
#define EXTI_FTSR     (*(volatile uint32_t *)(0x40010408))
#define EXTI_PR       (*(volatile uint32_t *)(0x40010414))
#define NVIC_ISER1    (*(volatile uint32_t *)(0xE000E104))

void exti_init(void) {
    // GPIOA, GPIOC ve AFIO saat sinyalleri
    RCC_APB2ENR |= (1 << 0) | (1 << 2) | (1 << 4);

    // PA5: Output (LED)
    GPIOA_CRL &= ~(0xF << 20);
    GPIOA_CRL |= (0x2 << 20);

    // PC13: Input Floating (Button)
    GPIOC_CRH &= ~(0xF << 20);
    GPIOC_CRH |= (0x4 << 20);

    // EXTI13 hattini Port C'ye yonlendir
    AFIO_EXTICR4 &= ~(0xF << 4);
    AFIO_EXTICR4 |= (0x2 << 4);

    // EXTI ayarları: Maskeleme kaldir ve dusen kenar tetikleme
    EXTI_IMR |= (1 << 13);
    EXTI_FTSR |= (1 << 13);

    // NVIC uzerinden EXTI15_10 kesmesini aktif et
    NVIC_ISER1 |= (1 << 8);
}

void EXTI15_10_IRQHandler(void) {
    if (EXTI_PR & (1 << 13)) {
        GPIOA_ODR ^= (1 << 5);
        EXTI_PR |= (1 << 13); // Bayragi temizle
    }
}

int main(void) {
    exti_init();

    while (1) {
        // Islemci baska gorevler yapabilir
    }
}
