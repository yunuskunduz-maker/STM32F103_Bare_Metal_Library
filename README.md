# STM32 Bare-Metal Driver Library (20 Projects) 🚀

---

## 📋 Project Purpose and Scope
**This repository contains 20 different driver projects developed entirely at the Register level for the STM32F103 (Cortex-M3) microcontroller, without using any high-level libraries (HAL, LL, or SPL).**

**The primary objective of this study is to gain a deep understanding of how hardware peripherals operate and to develop high-performance, optimized drivers.** **The projects cover communication protocols and hardware architectures critical for defense industry standards.**



---

## 📁 Project List

1.  **01_GPIO_Blink:** Basic LED control through RCC and GPIO register manipulation.
2.  **02_UART_Hello_World:** Introduction to asynchronous serial communication using the USART2 peripheral.
3.  **03_UART_Receiver:** Data reception from the external world using the polling method.
4.  **04_Timer_Basic_Delay:** Creating hardware delays using the general-purpose Timer (TIM2).
5.  **05_ADC_Single_Channel:** Reading sensor data using a 12-bit analog-to-digital converter.
6.  **06_PWM_LED_Brightness:** Brightness control via PWM signals generated through a timer.
7.  **07_I2C_Scanner_Master:** Detecting devices on the data bus using the I2C protocol.
8.  **08_SysTick_Delay:** Professional delay function utilizing the timer embedded in the ARM Cortex-M core.
9.  **09_SPI_Master_Driver:** High-speed SPI communication architecture and data transmission.
10. **10_External_Interrupt_EXTI:** External interrupt management with NVIC and EXTI units.
11. **11_Watchdog_IWDG:** Independent Watchdog Timer to protect against system hangs.
12. **12_Internal_Flash_Write:** Internal flash memory driver for data persistence during power loss.
13. **13_CRC_Calculation:** Hardware error checking for the verification of data packets.
14. **14_Power_Management:** Low power consumption strategies using Sleep Mode and the WFI instruction.
15. **15_Real_Time_Clock_RTC:** Hardware clock tracking with second-level precision using the LSE crystal.
16. **16_DMA_MemToMem:** High-speed memory-to-memory data transfer without CPU overhead.
17. **17_UART_DMA_Transmit:** Continuous UART data transfer supported by DMA.
18. **18_Timer_Input_Capture:** Measuring the frequency and period of incoming signals.
19. **19_PWM_Input_Measurement:** Full analysis of a PWM signal (Duty Cycle & Frequency) using a single pin.
20. **20_CAN_Bus_Base_Driver:** Driver for the CAN protocol, an aerospace and automotive industry standard.



---

## 🛠️ Technical Details
* **Hardware:** STM32F103RBT6 (Nucleo-F103RB)
* **Development Environment:** STM32CubeIDE (Bare-Metal C)
* **Architecture:** ARM® Cortex®-M3

---

**Developer:** Yunus Kunduz  
**YouTube:** [CozumLab](https://www.youtube.com/@CozumLabTR)
