#include <stdint.h>
#include <stdio.h>

// Projeyi gözlemlemek için bir potansiyometre bağlayınız.

// --- REGISTER ADRESLERİ ---
#define RCC_BASE      0x40021000
#define RCC_APB2ENR   (*(volatile uint32_t *)(RCC_BASE + 0x18))
#define RCC_APB1ENR   (*(volatile uint32_t *)(RCC_BASE + 0x1C))

#define GPIOA_BASE    0x40010800
#define GPIOA_CRL     (*(volatile uint32_t *)(GPIOA_BASE + 0x00))

// UART2 Registerları
#define USART2_BASE   0x40004400
#define USART2_SR     (*(volatile uint32_t *)(USART2_BASE + 0x00))
#define USART2_DR     (*(volatile uint32_t *)(USART2_BASE + 0x04))
#define USART2_BRR    (*(volatile uint32_t *)(USART2_BASE + 0x08))
#define USART2_CR1    (*(volatile uint32_t *)(USART2_BASE + 0x0C))

// ADC1 Registerları
#define ADC1_BASE     0x40012400
#define ADC1_SR       (*(volatile uint32_t *)(ADC1_BASE + 0x00))
#define ADC1_CR2      (*(volatile uint32_t *)(ADC1_BASE + 0x08))
#define ADC1_SMPR2    (*(volatile uint32_t *)(ADC1_BASE + 0x10))
#define ADC1_SQR3     (*(volatile uint32_t *)(ADC1_BASE + 0x34))
#define ADC1_DR       (*(volatile uint32_t *)(ADC1_BASE + 0x4C))

/* --- UART FONKSİYONLARI --- */

void UART2_Init(void) {
    RCC_APB2ENR |= (1 << 2);   // GPIOA Saatini Aç
    RCC_APB1ENR |= (1 << 17);  // USART2 Saatini Aç

    // PA2 (TX): Alt. Func. Out, PA3 (RX): Input
    GPIOA_CRL &= ~((0xF << 8) | (0xF << 12));
    GPIOA_CRL |= (0xB << 8) | (0x4 << 12);

    USART2_BRR = 0x45; // 115200 Baud @ 8MHz
    USART2_CR1 |= (1 << 13) | (1 << 3) | (1 << 2);
}

void UART2_Write(char ch) {
    while (!(USART2_SR & (1 << 7)));
    USART2_DR = (ch & 0xFF);
}

void UART2_SendString(char *str) {
    while (*str) UART2_Write(*str++);
}

/* --- ADC FONKSİYONLARI --- */

void ADC1_Init(void) {
    RCC_APB2ENR |= (1 << 9) | (1 << 2); // ADC1 ve GPIOA saatini aç
    GPIOA_CRL &= ~(0xF << 0);           // PA0 Analog Input (0000)

    ADC1_CR2 |= (1 << 0);               // ADC'yi Uyandır (ADON)

    // Kalibrasyon
    for(int i=0; i<1000; i++);
    ADC1_CR2 |= (1 << 2);               // Kalibrasyonu başlat
    while(ADC1_CR2 & (1 << 2));         // Bitene kadar bekle
}

uint16_t ADC1_Read(void) {
    ADC1_SQR3 = 0;                      // Kanal 0 seçimi
    ADC1_CR2 |= (1 << 0);               // Dönüşümü başlat
    while(!(ADC1_SR & (1 << 1)));       // End of Conversion bekle
    return (uint16_t)ADC1_DR;           // Veriyi döndür
}

/* --- ANA PROGRAM --- */

int main(void) {
    UART2_Init();
    ADC1_Init();

    char msg[64];
    UART2_SendString("\r\n--- CozumLab ADC Projesi Baslatildi ---\r\n");

    while (1) {
        uint16_t raw_value = ADC1_Read();

        // sprintf kullanarak formatlı yazı oluşturuyoruz
        sprintf(msg, "Analog Deger (PA0): %u\r\n", raw_value);
        UART2_SendString(msg);

        // Gözle görülür bir gecikme
        for(int i=0; i<1000000; i++);
    }
}
