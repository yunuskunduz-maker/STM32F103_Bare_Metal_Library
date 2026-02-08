#include <stdint.h>

#define RCC_APB1ENR   (*(volatile uint32_t *)0x4002101C)
#define PWR_CR        (*(volatile uint32_t *)0x40007000)
#define RCC_BDCR      (*(volatile uint32_t *)0x40021020)
#define RTC_CRL       (*(volatile uint32_t *)0x40002804)
#define RTC_CRH       (*(volatile uint32_t *)0x40002800)
#define RTC_PRLH      (*(volatile uint32_t *)0x40002808)
#define RTC_PRLL      (*(volatile uint32_t *)0x4000280C)
#define RTC_CNTH      (*(volatile uint32_t *)0x40002818)
#define RTC_CNTL      (*(volatile uint32_t *)0x4000281C)

void rtc_init(void) {
    RCC_APB1ENR |= (1 << 28) | (1 << 27); // PWR ve BKP saatleri
    PWR_CR |= (1 << 8);                   // Yedek bölge erisimi (DBP)

    RCC_BDCR |= (1 << 16);                // Backup domain reset
    RCC_BDCR &= ~(1 << 16);

    RCC_BDCR |= (1 << 0);                 // LSE (32.768 kHz) aktif
    while (!(RCC_BDCR & (1 << 1)));       // LSE hazir olana kadar bekle

    RCC_BDCR |= (1 << 8);                 // Saat kaynagi: LSE
    RCC_BDCR |= (1 << 15);                // RTC aktif

    while (!(RTC_CRL & (1 << 3)));        // Senkronizasyon bekle (RSF)
    while (!(RTC_CRL & (1 << 5)));        // Son yazma islemi bitisini bekle (RTOFF)

    RTC_CRL |= (1 << 4);                  // Yapilandirma modu (CNF)
    RTC_PRLH = 0;                         // Prescaler (32768 - 1)
    RTC_PRLL = 32767;
    RTC_CRL &= ~(1 << 4);                 // Yapilandirma modundan cik
    while (!(RTC_CRL & (1 << 5)));        // RTOFF bekle
}

uint32_t rtc_get_time(void) {
    return ((uint32_t)RTC_CNTH << 16) | RTC_CNTL;
}

void rtc_set_time(uint32_t time) {
    while (!(RTC_CRL & (1 << 5)));
    RTC_CRL |= (1 << 4);
    RTC_CNTH = (time >> 16);
    RTC_CNTL = (time & 0xFFFF);
    RTC_CRL &= ~(1 << 4);
    while (!(RTC_CRL & (1 << 5)));
}

int main(void) {
    rtc_init();

    // Örnek: Saati 12:00:00 (43200 saniye) olarak ayarla
    rtc_set_time(43200);

    while (1) {
        uint32_t current_time = rtc_get_time();
        // Zaman takibi
    }
}
