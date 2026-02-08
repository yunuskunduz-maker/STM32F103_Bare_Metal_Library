#include <stdint.h>


// Saat Kontrol (RCC)
#define RCC_BASE      0x40021000
#define RCC_APB2ENR   (*(volatile uint32_t *)(RCC_BASE + 0x18)) // GPIOA için
#define RCC_APB1ENR   (*(volatile uint32_t *)(RCC_BASE + 0x1C)) // USART2 için

// GPIOA (PA2: TX, PA3: RX)
#define GPIOA_BASE    0x40010800
#define GPIOA_CRL     (*(volatile uint32_t *)(GPIOA_BASE + 0x00))

// USART2
#define USART2_BASE   0x40004400
#define USART2_SR     (*(volatile uint32_t *)(USART2_BASE + 0x00)) // Status
#define USART2_DR     (*(volatile uint32_t *)(USART2_BASE + 0x04)) // Data
#define USART2_BRR    (*(volatile uint32_t *)(USART2_BASE + 0x08)) // Baud Rate
#define USART2_CR1    (*(volatile uint32_t *)(USART2_BASE + 0x0C)) // Control

/*
 * USART2 Birimini Başlatır
 * Hız: 115200 Baud, 8-bit, 1 Stop Bit
 */
void UART2_Init(void) {
    // 1. Saat Sinyallerini Aktif Et
    RCC_APB2ENR |= (1 << 2);
    RCC_APB1ENR |= (1 << 17);

    GPIOA_CRL &= ~((0xF << 8) | (0xF << 12)); // Bitleri Temizle
    GPIOA_CRL |= (0xB << 8) | (0x4 << 12);   // Modları Ata

    USART2_BRR = 0x45;

    USART2_CR1 |= (1 << 13) | (1 << 3) | (1 << 2);
}


void UART2_Write(char ch) {
    while (!(USART2_SR & (1 << 7)));
    USART2_DR = (ch & 0xFF);
}

// Polling yöntemi

char UART2_Read(void) {
    while (!(USART2_SR & (1 << 5)));
    return (char)(USART2_DR & 0xFF);
}


void UART2_SendString(char *str) {
    while (*str) {
        UART2_Write(*str++);
    }
}


int main(void) {
    UART2_Init();

    UART2_SendString("\r\n02 UART Polling Deneyine Hosgeldiniz!\r\n");
    UART2_SendString("Yazdiginiz her sey geri gonderilecektir (Echo).\r\n");

    while (1) {
        char received = UART2_Read();

        UART2_Write(received);
    }
}
