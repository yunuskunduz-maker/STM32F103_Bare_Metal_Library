#include <stdint.h>

#define RCC_AHBENR    (*(volatile uint32_t *)0x40021014)
#define CRC_DR        (*(volatile uint32_t *)0x40003000)
#define CRC_CR        (*(volatile uint32_t *)0x40003008)

void crc_init(void) {
    RCC_AHBENR |= (1 << 6); // CRC Saatini ac
    CRC_CR |= (1 << 0);     // Resetle
}

uint32_t crc_calculate(uint32_t data) {
    CRC_DR = data;
    return CRC_DR;
}

int main(void) {
    uint32_t my_data = 0x12345678;
    uint32_t checksum = 0;

    crc_init();
    checksum = crc_calculate(my_data);

    while (1) {
        // Checksum validation
    }
}
