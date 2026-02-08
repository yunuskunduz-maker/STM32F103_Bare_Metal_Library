# STM32 Bare-Metal Sürücü Kütüphanesi (20 Proje)

Bu depo, STM32F103 (Cortex-M3) mikrodenetleyicisi üzerinde, herhangi bir hazır kütüphane (HAL, LL veya SPL) kullanılmadan, tamamen **Register (Kayıtçı)** seviyesinde geliştirilmiş 20 farklı sürücü projesini içermektedir.

## 🚀 Proje Amacı ve Kapsamı
Bu çalışma, gömülü sistemlerin kalbine inerek donanım birimlerinin (peripherals) nasıl çalıştığını derinlemesine anlamak ve yüksek performanslı, optimize edilmiş sürücüler geliştirmek amacıyla hazırlanmıştır. Projeler, savunma sanayii standartlarında kritik öneme sahip olan haberleşme protokollerini ve donanım mimarilerini kapsamaktadır.

## 📁 Proje Listesi

1.  **01_GPIO_Blink:** RCC ve GPIO register manipülasyonu ile temel LED kontrolü.
2.  **02_UART_Hello_World:** USART2 birimi ile asenkron seri haberleşme başlangıcı.
3.  **03_UART_Receiver:** Polling yöntemiyle dış dünyadan veri alımı.
4.  **04_Timer_Basic_Delay:** Genel amaçlı Timer (TIM2) ile donanımsal gecikme oluşturma.
5.  **05_ADC_Single_Channel:** 12-bit analog-dijital çevirici ile sensör verisi okuma.
6.  **06_PWM_LED_Brightness:** Timer üzerinden PWM sinyali ile parlaklık kontrolü.
7.  **07_I2C_Scanner_Master:** I2C protokolü ile veri yolu üzerindeki cihazların tespiti.
8.  **08_SysTick_Delay:** ARM Cortex-M çekirdeğine gömülü zamanlayıcı ile profesyonel delay fonksiyonu.
9.  **09_SPI_Master_Driver:** Yüksek hızlı SPI haberleşme mimarisi ve veri iletimi.
10. **10_External_Interrupt_EXTI:** NVIC ve EXTI birimleri ile harici kesme yönetimi.
11. **11_Watchdog_IWDG:** Sistem kilitlenmelerine karşı bağımsız bekçi zamanlayıcısı.
12. **12_Internal_Flash_Write:** Enerji kesilse dahi verileri saklayan dahili flash bellek sürücüsü.
13. **13_CRC_Calculation:** Veri paketlerinin doğrulanması için donanımsal hata kontrolü.
14. **14_Power_Management:** Sleep Mode ve WFI komutu ile düşük güç tüketimi.
15. **15_Real_Time_Clock_RTC:** LSE kristali ile saniye hassasiyetinde donanımsal saat takibi.
16. **16_DMA_MemToMem:** İşlemciyi yormadan bellekler arası yüksek hızlı veri taşıma.
17. **17_UART_DMA_Transmit:** DMA destekli, kesintisiz UART veri transferi.
18. **18_Timer_Input_Capture:** Gelen sinyallerin frekans ve periyodunun ölçümü.
19. **19_PWM_Input_Measurement:** Tek bir pin ile PWM sinyalinin (Duty & Freq) tam analizi.
20. **20_CAN_Bus_Base_Driver:** Havacılık ve otomotiv standardı olan CAN protokolü sürücüsü.

## 🛠️ Teknik Detaylar
- **Donanım:** STM32F103RBT6 (Nucleo-F103RB)
- **Geliştirme Ortamı:** STM32CubeIDE (Bare-Metal C)
- **Mimari:** ARM Cortex-M3

---
**Geliştiren:** [Yunus Kunduz]
