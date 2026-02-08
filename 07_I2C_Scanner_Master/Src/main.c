#include <stdint.h>
#include <stdio.h>

// --- REGISTER ADRESLERİ ---
#define RCC_BASE      0x40021000
#define RCC_APB2ENR   (*(volatile uint32_t *)(RCC_BASE + 0x18))
#define RCC_APB1ENR   (*(volatile uint32_t *)(RCC_BASE + 0x1C))

#define GPIOB_BASE    0x40010C00
#define GPIOB_CRL     (*(volatile uint32_t *)(GPIOB_BASE + 0x00))

// I2C1 Registerları
#define I2C1_BASE     0x40005400
#define I2C1_CR1      (*(volatile uint32_t *)(I2C1_BASE + 0x00))
#define I2C1_CR2      (*(volatile uint32_t *)(I2C1_BASE + 0x04))
#define I2C1_DR       (*(volatile uint32_t *)(I2C1_BASE + 0x10))
#define I2C1_SR1      (*(volatile uint32_t *)(I2C1_BASE + 0x14))
#define I2C1_SR2      (*(volatile uint32_t *)(I2C1_BASE + 0x18))
#define I2C1_CCR      (*(volatile uint32_t *)(I2C1_BASE + 0x1C))
#define I2C1_TRISE    (*(volatile uint32_t *)(I2C1_BASE + 0x20))

/* --- UART Sürücüsü (Hızlıca üzerinden geçiyoruz) --- */
void UART2_Init(void) {
    RCC_APB2ENR |= (1 << 2); RCC_APB1ENR |= (1 << 17);
    *(volatile uint32_t *)0x40010800 &= ~((0xF << 8) | (0xF << 12));
    *(volatile uint32_t *)0x40010800 |= (0xB << 8) | (0x4 << 12);
    *(volatile uint32_t *)0x40004408 = 0x45;
    *(volatile uint32_t *)0x4000440C |= (1 << 13) | (1 << 3) | (1 << 2);
}
void UART2_SendString(char *s) { while(*s) { while(!(*(volatile uint32_t *)0x40004400 & (1 << 7))); *(volatile uint32_t *)0x40004404 = *s++; } }

/* --- I2C BAŞLATMA VE TARAMA --- */

void I2C1_Init(void) {
    RCC_APB2ENR |= (1 << 3);  // GPIOB Saatini aç
    RCC_APB1ENR |= (1 << 21); // I2C1 Saatini aç

    // PB6 ve PB7: Alternate Function Open-Drain, 50MHz (1111 -> 0xF)
    GPIOB_CRL &= ~((0xF << 24) | (0xF << 28));
    GPIOB_CRL |= (0xF << 24) | (0xF << 28);

    I2C1_CR1 |= (1 << 15); // I2C Resetle
    I2C1_CR1 &= ~(1 << 15);

    I2C1_CR2 = 8;          // 8 MHz çevresel saat frekansı
    I2C1_CCR = 40;         // 100 kHz Standart Mode (8MHz / (2 * 100kHz))
    I2C1_TRISE = 9;        // Maksimum yükselme süresi (8MHz + 1)

    I2C1_CR1 |= (1 << 0);  // I2C Aktif Et (PE bit)
}

int I2C_CheckAddress(uint8_t addr) {
    I2C1_CR1 |= (1 << 8);  // START sinyali gönder
    while (!(I2C1_SR1 & (1 << 0))); // Start bitinin (SB) gönderilmesini bekle

    I2C1_DR = (addr << 1); // Adresi yaz (Write bit 0)

    // ADDR bitinin gelmesini veya hata (AF - Acknowledge Failure) olmasını bekle
    while (!(I2C1_SR1 & (1 << 1))) {
        if (I2C1_SR1 & (1 << 10)) { // AF (Hata) gelirse
            I2C1_SR1 &= ~(1 << 10); // Hatayı temizle
            I2C1_CR1 |= (1 << 9);   // STOP gönder
            return 0; // Cihaz yok
        }
    }

    (void)I2C1_SR2;         // ADDR bitini temizlemek için SR2 oku
    I2C1_CR1 |= (1 << 9);   // STOP gönder
    return 1;               // Cihaz bulundu!
}

int main(void) {
    UART2_Init();
    I2C1_Init();
    char buf[32];
    UART2_SendString("\r\nI2C Bus Taraniyor...\r\n");

    for (uint8_t i = 1; i < 128; i++) {
        if (I2C_CheckAddress(i)) {
            sprintf(buf, "Cihaz Bulundu: 0x%02X\r\n", i);
            UART2_SendString(buf);
        }
    }
    UART2_SendString("Tarama Tamamlandi.\r\n");

    while (1);
}
