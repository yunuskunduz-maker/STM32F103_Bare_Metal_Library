#include <stdint.h>

#define RCC_BASE      0x40021000
#define RCC_APB2ENR   (*(volatile uint32_t *)(RCC_BASE + 0x18))

#define GPIOA_BASE    0x40010800
#define GPIOA_CRL     (*(volatile uint32_t *)(GPIOA_BASE + 0x00))

#define SPI1_BASE     0x40013000
#define SPI1_CR1      (*(volatile uint32_t *)(SPI1_BASE + 0x00))
#define SPI1_SR       (*(volatile uint32_t *)(SPI1_BASE + 0x08))
#define SPI1_DR       (*(volatile uint32_t *)(SPI1_BASE + 0x0C))

void spi1_init(void) {
    RCC_APB2ENR |= (1 << 12) | (1 << 2);

    // PA5: SCK (AF-PP), PA6: MISO (Input), PA7: MOSI (AF-PP)
    GPIOA_CRL &= ~((0xF << 20) | (0xF << 24) | (0xF << 28));
    GPIOA_CRL |= (0xB << 20) | (0x4 << 24) | (0xB << 28);

    // BR: fPCLK/8, CPOL: 0, CPHA: 0, DFF: 8-bit, SSM: 1, SSI: 1, MSTR: 1
    SPI1_CR1 |= (2 << 3) | (1 << 9) | (1 << 8) | (1 << 2);
    SPI1_CR1 |= (1 << 6);
}

uint8_t spi1_transmit(uint8_t data) {
    while (!(SPI1_SR & (1 << 1)));
    SPI1_DR = data;

    while (!(SPI1_SR & (1 << 0)));
    return (uint8_t)SPI1_DR;
}

int main(void) {
    spi1_init();

    while (1) {
        spi1_transmit(0xAA);
        for (volatile int i = 0; i < 5000; i++);
    }
}
