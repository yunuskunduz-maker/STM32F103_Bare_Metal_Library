#include <stdint.h>

// --- REGISTER ADRESLERİ ---
// SysTick birimi işlemci çekirdeğine bağlıdır (SCS - System Control Space)
#define STK_CTRL      (*(volatile uint32_t *)(0xE000E010)) // Kontrol ve Durum
#define STK_LOAD      (*(volatile uint32_t *)(0xE000E014)) // Yükleme Değeri
#define STK_VAL       (*(volatile uint32_t *)(0xE000E018)) // Mevcut Değer

// GPIO ve RCC (LED Testi İçin)
#define RCC_APB2ENR   (*(volatile uint32_t *)(0x40021018))
#define GPIOA_CRL     (*(volatile uint32_t *)(0x40010800))
#define GPIOA_ODR     (*(volatile uint32_t *)(0x4001080C))

/* --- GLOBAL DEĞİŞKENLER --- */
// volatile kullanımı kritiktir: Kesme içinde değişen değerleri derleyiciye bildirir
static volatile uint32_t ms_ticks = 0;

/* --- FONKSİYONLAR --- */

/**
 * SysTick Birimini 1ms'de bir kesme üretecek şekilde kurar.
 * @param sclk: Sistem saat frekansı (HSI varsayılan 8.000.000)
 */
void SysTick_Init(uint32_t sclk) {
    // 1ms için gereken sayım: sclk / 1000
    // 8MHz için -> 8000 tick
    STK_LOAD = (sclk / 1000) - 1;

    // Mevcut sayacı temizle
    STK_VAL = 0;

    // Ayarlar:
    // Bit 2: Saat kaynağı (1: İşlemci hızı)
    // Bit 1: Kesme (Interrupt) aktif (1: Aktif)
    // Bit 0: Sayacı başlat (1: Başlat)
    STK_CTRL = (1 << 2) | (1 << 1) | (1 << 0);
}

/**
 * SysTick Kesme Servis Rutini (ISR)
 * İşlemci donanımsal olarak her 1ms'de bir buraya atlar.
 */
void SysTick_Handler(void) {
    ms_ticks++;
}

/**
 * Milisaniye hassasiyetinde gecikme
 */
void delay_ms(uint32_t ms) {
    uint32_t start_tick = ms_ticks;
    while ((ms_ticks - start_tick) < ms);
}

/* --- ANA PROGRAM --- */

int main(void) {
    // 1. LED Ayarı (PA5)
    RCC_APB2ENR |= (1 << 2);
    GPIOA_CRL &= ~(0xF << 20);
    GPIOA_CRL |= (0x2 << 20);

    // 2. SysTick Başlat (8MHz saat hızı için)
    SysTick_Init(8000000);

    while (1) {
        GPIOA_ODR ^= (1 << 5); // LED Durumunu Değiştir

        // Artık bu gecikme donanımsal olarak 1ms hassasiyetindedir
        delay_ms(500);
    }
}
