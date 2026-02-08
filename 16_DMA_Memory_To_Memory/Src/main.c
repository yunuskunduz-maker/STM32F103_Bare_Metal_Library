#include <stdint.h>

#define RCC_AHBENR       (*(volatile uint32_t *)0x40021014)

#define DMA1_ISR         (*(volatile uint32_t *)0x40020000)
#define DMA1_IFCR        (*(volatile uint32_t *)0x40020004)
#define DMA1_CCR1        (*(volatile uint32_t *)0x40020008)
#define DMA1_CNDTR1      (*(volatile uint32_t *)0x4002000C)
#define DMA1_CPAR1       (*(volatile uint32_t *)0x40020010)
#define DMA1_CMAR1       (*(volatile uint32_t *)0x40020014)

#define BUFFER_SIZE      8

uint32_t src_buffer[BUFFER_SIZE] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88};
uint32_t dst_buffer[BUFFER_SIZE] = {0};

void dma_init(void) {
    RCC_AHBENR |= (1 << 0); // DMA1 saatini aktif et

    DMA1_CPAR1 = (uint32_t)src_buffer; // Kaynak adresi
    DMA1_CMAR1 = (uint32_t)dst_buffer; // Hedef adresi
    DMA1_CNDTR1 = BUFFER_SIZE;         // Veri miktari

    /*
     * M2M: 1 (Memory to Memory)
     * PL: 10 (High Priority)
     * MSIZE: 10 (32-bit)
     * PSIZE: 10 (32-bit)
     * MINC: 1 (Memory increment enabled)
     * PINC: 1 (Peripheral increment enabled)
     * DIR: 0 (Read from peripheral/source)
     * TCIE: 1 (Transfer complete interrupt enable)
     * EN: 1 (Enable)
     */
    DMA1_CCR1 = (1 << 14) | (2 << 12) | (2 << 10) | (2 << 8) | (1 << 7) | (1 << 6) | (1 << 1);
}

void dma_start(void) {
    DMA1_CCR1 |= (1 << 0);
}

int main(void) {
    dma_init();
    dma_start();

    // Transferin bitmesini bekle (TCIF1)
    while (!(DMA1_ISR & (1 << 1)));

    // Bayragi temizle
    DMA1_IFCR |= (1 << 1);

    while (1) {
        // dst_buffer icerigi src_buffer ile ayni olmali
    }
}
