#include <stdint.h>

#define FLASH_BASE    0x40022000
#define FLASH_ACR     (*(volatile uint32_t *)(FLASH_BASE + 0x00))
#define FLASH_KEYR    (*(volatile uint32_t *)(FLASH_BASE + 0x04))
#define FLASH_SR      (*(volatile uint32_t *)(FLASH_BASE + 0x0C))
#define FLASH_CR      (*(volatile uint32_t *)(FLASH_BASE + 0x10))
#define FLASH_AR      (*(volatile uint32_t *)(FLASH_BASE + 0x14))

#define FLASH_KEY1    0x45670123
#define FLASH_KEY2    0xCDEF89AB
#define TARGET_ADDR   0x0801FC00 // F103RB için son sayfa baslangici

void flash_unlock(void) {
    if (FLASH_CR & (1 << 7)) { // LOCK biti kontrolü
        FLASH_KEYR = FLASH_KEY1;
        FLASH_KEYR = FLASH_KEY2;
    }
}

void flash_lock(void) {
    FLASH_CR |= (1 << 7);
}

void flash_erase_page(uint32_t addr) {
    while (FLASH_SR & (1 << 0)); // BSY kontrolü
    FLASH_CR |= (1 << 1);        // PER (Page Erase) aktif
    FLASH_AR = addr;
    FLASH_CR |= (1 << 6);        // STRT (Start)
    while (FLASH_SR & (1 << 0));
    FLASH_CR &= ~(1 << 1);       // PER pasif
}

void flash_write_halfword(uint32_t addr, uint16_t data) {
    while (FLASH_SR & (1 << 0));
    FLASH_CR |= (1 << 0);        // PG (Programming) aktif
    *(volatile uint16_t *)addr = data;
    while (FLASH_SR & (1 << 0));
    FLASH_CR &= ~(1 << 0);       // PG pasif
}

int main(void) {
    uint16_t data_to_store = 0xABCD;
    uint16_t read_data = 0;

    flash_unlock();
    flash_erase_page(TARGET_ADDR);
    flash_write_halfword(TARGET_ADDR, data_to_store);
    flash_lock();

    // Veriyi oku
    read_data = *(volatile uint16_t *)TARGET_ADDR;

    while (1) {
        if (read_data == 0xABCD) {
            // Basarili yazma islemi
        }
    }
}
