#include <stdint.h>

#define RCC_AHBENR    (*(volatile uint32_t *)0x40021014)
#define RCC_APB2ENR   (*(volatile uint32_t *)0x40021018)
#define RCC_APB1ENR   (*(volatile uint32_t *)0x4002101C)

#define GPIOA_CRL     (*(volatile uint32_t *)0x40010800)

#define USART2_SR     (*(volatile uint32_t *)0x40004400)
#define USART2_DR     (*(volatile uint32_t *)0x40004404)
#define USART2_BRR    (*(volatile uint32_t *)0x40004408)
#define USART2_CR1    (*(volatile uint32_t *)0x4000440C)
#define USART2_CR3    (*(volatile uint32_t *)0x40004414)

#define DMA1_ISR      (*(volatile uint32_t *)0x40020000)
#define DMA1_IFCR     (*(volatile uint32_t *)0x40020004)
#define DMA1_CCR7     (*(volatile uint32_t *)0x40020080)
#define DMA1_CNDTR7   (*(volatile uint32_t *)0x40020084)
#define DMA1_CPAR7    (*(volatile uint32_t *)0x40020088)
#define DMA1_CMAR7    (*(volatile uint32_t *)0x4002008C)

char msg[] = "CozumLab DMA UART Transfer Active\r\n";

void uart_dma_init(void) {
    RCC_AHBENR  |= (1 << 0);  // DMA1
    RCC_APB2ENR |= (1 << 2);  // GPIOA
    RCC_APB1ENR |= (1 << 17); // USART2

    // PA2 TX: AF-PP
    GPIOA_CRL &= ~(0xF << 8);
    GPIOA_CRL |= (0xB << 8);

    USART2_BRR = 0x45; // 115200 Baud @ 8MHz
    USART2_CR3 |= (1 << 7); // DMAT (DMA Transmit Enable)
    USART2_CR1 |= (1 << 13) | (1 << 3);

    // DMA1 Channel 7 (USART2_TX)
    DMA1_CPAR7 = (uint32_t)&USART2_DR;
    // DIR: 1 (Mem to Periph), MINC: 1, TCIE: 1
    DMA1_CCR7 = (1 << 4) | (1 << 7) | (1 << 1);
}

void uart_dma_send(char *data, uint16_t size) {
    DMA1_CCR7 &= ~(1 << 0); // Disable
    DMA1_CMAR7 = (uint32_t)data;
    DMA1_CNDTR7 = size;
    DMA1_IFCR |= (1 << 24); // Clear TCIF7
    DMA1_CCR7 |= (1 << 0);  // Enable
}

int main(void) {
    uart_dma_init();

    while (1) {
        uart_dma_send(msg, sizeof(msg) - 1);

        // Wait for TCIF7
        while (!(DMA1_ISR & (1 << 24)));

        for (volatile int i = 0; i < 2000000; i++);
    }
}
