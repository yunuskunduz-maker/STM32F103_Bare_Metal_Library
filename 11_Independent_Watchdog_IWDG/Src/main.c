#include <stdint.h>

#define RCC_APB2ENR   (*(volatile uint32_t *)(0x40021018))
#define GPIOA_CRL     (*(volatile uint32_t *)(0x40010800))
#define GPIOA_ODR     (*(volatile uint32_t *)(0x4001080C))

#define IWDG_KR       (*(volatile uint32_t *)(0x40003000))
#define IWDG_PR       (*(volatile uint32_t *)(0x40003004))
#define IWDG_RLR      (*(volatile uint32_t *)(0x40003008))

void iwdg_init(void) {
    IWDG_KR  = 0x5555; // Register erisimini ac
    IWDG_PR  = 0x04;   // Prescaler /64
    IWDG_RLR = 625;    // ~1 saniye timeout (40kHz / 64 = 625Hz)
    IWDG_KR  = 0xAAAA; // Sayaci yenile (Refresh)
    IWDG_KR  = 0xCCCC; // Watchdog baslat
}

void iwdg_feed(void) {
    IWDG_KR = 0xAAAA; // "besle"
}

int main(void) {
    RCC_APB2ENR |= (1 << 2);
    GPIOA_CRL &= ~(0xF << 20);
    GPIOA_CRL |= (0x2 << 20);

    // Baslangicta LED yak (Reset atildigini gormek icin)
    GPIOA_ODR |= (1 << 5);
    for(volatile int i = 0; i < 500000; i++);
    GPIOA_ODR &= ~(1 << 5);

    iwdg_init();

    while (1) {
        // Normal calismada besle
        iwdg_feed();

        // Eger bu satiri yoruma alirsan veya kod burada takilirsa
        // islemci 1 saniye sonra reset atar ve LED tekrar yanar.
        for(volatile int i = 0; i < 100000; i++);
    }
}
