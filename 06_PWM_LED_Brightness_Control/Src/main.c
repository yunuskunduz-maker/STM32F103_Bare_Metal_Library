#include <stdint.h>

/* --- REGISTER ADRESLERİ --- */
#define RCC_BASE      0x40021000
#define RCC_APB2ENR   (*(volatile uint32_t *)(RCC_BASE + 0x18))
#define RCC_APB1ENR   (*(volatile uint32_t *)(RCC_BASE + 0x1C))

#define GPIOA_BASE    0x40010800
#define GPIOA_CRL     (*(volatile uint32_t *)(GPIOA_BASE + 0x00))

// Timer 2 (PWM için)
#define TIM2_BASE     0x40000000
#define TIM2_CR1      (*(volatile uint32_t *)(TIM2_BASE + 0x00))
#define TIM2_CCMR1    (*(volatile uint32_t *)(TIM2_BASE + 0x18))
#define TIM2_CCER     (*(volatile uint32_t *)(TIM2_BASE + 0x20))
#define TIM2_PSC      (*(volatile uint32_t *)(TIM2_BASE + 0x28))
#define TIM2_ARR      (*(volatile uint32_t *)(TIM2_BASE + 0x2C))
#define TIM2_CCR1     (*(volatile uint32_t *)(TIM2_BASE + 0x34))

// ADC1 (Analog Okuma için)
#define ADC1_BASE     0x40012400
#define ADC1_SR       (*(volatile uint32_t *)(ADC1_BASE + 0x00))
#define ADC1_CR2      (*(volatile uint32_t *)(ADC1_BASE + 0x08))
#define ADC1_SQR3     (*(volatile uint32_t *)(ADC1_BASE + 0x34))
#define ADC1_DR       (*(volatile uint32_t *)(ADC1_BASE + 0x4C))

/* --- SİSTEM BAŞLATMA FONKSİYONLARI --- */

void System_Init(void) {
    // 1. Saat Sinyallerini Aç (GPIOA, TIM2, ADC1)
    RCC_APB2ENR |= (1 << 2) | (1 << 9); // GPIOA ve ADC1
    RCC_APB1ENR |= (1 << 0);           // TIM2

    // 2. Pin Ayarları
    // PA0: Analog Input (0000) - Potansiyometre girişi
    // PA5: Alternate Function Output Push-Pull (1011 -> 0xB) - LED çıkışı
    GPIOA_CRL &= ~((0xF << 0) | (0xF << 20)); // Önce temizle
    GPIOA_CRL |= (0xB << 20);                // PA5'i PWM çıkışı yap
}

void ADC1_Init(void) {
    ADC1_CR2 |= (1 << 0);           // ADC Uyandır
    for(int i=0; i<1000; i++);      // Stabilizasyon beklemesi
    ADC1_CR2 |= (1 << 2);           // Kalibrasyonu Başlat
    while(ADC1_CR2 & (1 << 2));     // Kalibrasyon bitene kadar bekle
}

void PWM_Init(void) {
    TIM2_PSC = 0;                   // En hızlı sayım
    TIM2_ARR = 4095;                // Periyot 4095 (ADC ile birebir eşleşsin)

    // PWM Mode 1: Kanal 1 için (OC1M = 110)
    TIM2_CCMR1 |= (6 << 4);

    // Kanal 1 çıkışını aktif et
    TIM2_CCER |= (1 << 0);

    // Sayacı başlat
    TIM2_CR1 |= (1 << 0);
}

uint16_t ADC1_Read(void) {
    ADC1_SQR3 = 0;                  // Kanal 0 (PA0)
    ADC1_CR2 |= (1 << 0);           // Dönüşümü Başlat
    while(!(ADC1_SR & (1 << 1)));   // End of Conversion (EOC) bekle
    return (uint16_t)ADC1_DR;
}

/* --- ANA PROGRAM --- */

int main(void) {
    System_Init();
    ADC1_Init();
    PWM_Init();

    while (1) {
        // 1. Potansiyometre değerini oku (0 - 4095)
        uint16_t adc_val = ADC1_Read();

        // 2. Değeri PWM doluluk oranına (Duty Cycle) aktar
        // CCR1 değeri 0 ise LED söner, 4095 ise maksimum parlaklıkta yanar.
        TIM2_CCR1 = adc_val;
    }
}
