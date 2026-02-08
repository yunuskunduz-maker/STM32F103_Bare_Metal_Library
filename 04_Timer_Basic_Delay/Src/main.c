#include <stdint.h>

// --- ADRES TANIMLAMALARI ---
#define RCC_APB1ENR   (*(volatile uint32_t *)(0x4002101C))
#define RCC_APB2ENR   (*(volatile uint32_t *)(0x40021018))
#define GPIOA_CRL     (*(volatile uint32_t *)(0x40010800))
#define GPIOA_ODR     (*(volatile uint32_t *)(0x4001080C))

// TIM2 Registerları
#define TIM2_BASE     0x40000000
#define TIM2_CR1      (*(volatile uint32_t *)(TIM2_BASE + 0x00))
#define TIM2_SR       (*(volatile uint32_t *)(TIM2_BASE + 0x10))
#define TIM2_EGR      (*(volatile uint32_t *)(TIM2_BASE + 0x14))
#define TIM2_CNT      (*(volatile uint32_t *)(TIM2_BASE + 0x24)) // Buraya çektik!
#define TIM2_PSC      (*(volatile uint32_t *)(TIM2_BASE + 0x28))
#define TIM2_ARR      (*(volatile uint32_t *)(TIM2_BASE + 0x2C))

/* --- FONKSİYONLAR --- */

void Timer2_Init(void) {
    // 1. TIM2 Saatini Aç
    RCC_APB1ENR |= (1 << 0);

    // 2. Prescaler Ayarı: 8MHz / 8000 = 1kHz (Her sayım 1ms sürer)
    TIM2_PSC = 7999;

    // 3. Auto-reload: Maksimum değer (65535)
    TIM2_ARR = 0xFFFF;

    // 4. Update Event oluştur ve Sayacı başlat
    TIM2_EGR |= (1 << 0);
    TIM2_CR1 |= (1 << 0);
}

// Milisaniye cinsinden donanımsal bekleme
void delay_ms(uint16_t ms) {
    TIM2_CNT = 0;           // Sayacı sıfırla
    while (TIM2_CNT < ms);  // Sayaç hedef ms değerine ulaşana kadar bekle
}

int main(void) {
    // GPIOA ve LED ayarı (PA5 - Yeşil LED)
    RCC_APB2ENR |= (1 << 2);
    GPIOA_CRL &= ~(0xF << 20);
    GPIOA_CRL |= (0x2 << 20);

    Timer2_Init();

    while (1) {
        GPIOA_ODR ^= (1 << 5); // LED'i tersle (Toggle)
        delay_ms(1000);        // Tam 1 saniye bekle
    }
}
