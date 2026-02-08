#include <stdint.h>

/* --- REGISTER ADRESLERİ (Aynı Kalıyor) --- */
#define RCC_APB2ENR   (*(volatile uint32_t *)(0x40021018))
#define RCC_APB1ENR   (*(volatile uint32_t *)(0x4002101C))
#define GPIOA_CRL     (*(volatile uint32_t *)(0x40010800))
#define GPIOA_ODR     (*(volatile uint32_t *)(0x4001080C))
#define USART2_SR     (*(volatile uint32_t *)(0x40004400))
#define USART2_DR     (*(volatile uint32_t *)(0x40004404))
#define USART2_BRR    (*(volatile uint32_t *)(0x40004408))
#define USART2_CR1    (*(volatile uint32_t *)(0x4000440C))
#define NVIC_ISER1    (*(volatile uint32_t *)(0xE000E104))

/* --- YARDIMCI FONKSİYONLAR --- */

void UART2_Write(char ch) {
    while (!(USART2_SR & (1 << 7)));
    USART2_DR = (ch & 0xFF);
}

void UART2_SendString(char *str) {
    while (*str) UART2_Write(*str++);
}

// Kullanıcıya ne yapacağını hatırlatan "Prompt" fonksiyonu
void CLI_ShowPrompt(void) {
    UART2_SendString("\r\nSTM32_BareMetal> ");
}

void CLI_ShowHelp(void) {
    UART2_SendString("\r\n--- MEVCUT KOMUTLAR ---");
    UART2_SendString("\r\n[A] : Yeşil LED'i YAK");
    UART2_SendString("\r\n[S] : Yeşil LED'i SONDUR");
    UART2_SendString("\r\n[H] : Bu yardim menusunu goster");
    UART2_SendString("\r\n-----------------------\r\n");
}

/* --- KESME SERVİS RUTİNİ --- */

void USART2_IRQHandler(void) {
    if (USART2_SR & (1 << 5)) { // Veri geldi mi?
        char key = (char)(USART2_DR & 0xFF);

        // Enter veya Satır sonu karakterlerini görmezden gel
        if (key == '\r' || key == '\n') return;

        UART2_Write(key); // Bastığı tuşu ekrana geri yaz (Echo)
        UART2_SendString("\r\n"); // Alt satıra geç

        if (key == 'A' || key == 'a') {
            GPIOA_ODR |= (1 << 5);
            UART2_SendString("[BILGI]: LED simdi ACIK.");
        }
        else if (key == 'S' || key == 's') {
            GPIOA_ODR &= ~(1 << 5);
            UART2_SendString("[BILGI]: LED simdi KAPALI.");
        }
        else if (key == 'H' || key == 'h') {
            CLI_ShowHelp();
        }
        else {
            UART2_SendString("[HATA]: Gecersiz komut! Yardim icin 'H' tusuna basin.");
        }

        CLI_ShowPrompt(); // Her işlemden sonra yeni satırda prompt göster
    }
}

/* --- ANA PROGRAM --- */

void UART2_Interrupt_Init(void) {
    RCC_APB2ENR |= (1 << 2);
    RCC_APB1ENR |= (1 << 17);
    GPIOA_CRL &= ~((0xF << 8) | (0xF << 12) | (0xF << 20));
    GPIOA_CRL |= (0xB << 8) | (0x4 << 12) | (0x2 << 20);
    USART2_BRR = 0x45;
    USART2_CR1 |= (1 << 13) | (1 << 3) | (1 << 2) | (1 << 5);
    NVIC_ISER1 |= (1 << 6);
}

int main(void) {
    UART2_Interrupt_Init();

    // Sistemin açıldığını ve ne yapılması gerektiğini ilk başta gösteriyoruz
    UART2_SendString("\033[2J\033[H"); // Terminal ekranını temizleme kodu (opsiyonel)
    UART2_SendString("STM32 Bare Metal Sistemi Aktif!");
    CLI_ShowHelp();
    CLI_ShowPrompt();

    while (1) {
        // İşlemci burada başka işler yapabilir...
    }
}
