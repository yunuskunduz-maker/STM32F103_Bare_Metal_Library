#include <stdint.h>

#define RCC_APB2ENR   (*(volatile uint32_t *)0x40021018)
#define RCC_APB1ENR   (*(volatile uint32_t *)0x4002101C)
#define GPIOA_CRH     (*(volatile uint32_t *)0x40010804)

/* CAN Control ve Status Registerları */
#define CAN_MCR       (*(volatile uint32_t *)0x40006400)
#define CAN_MSR       (*(volatile uint32_t *)0x40006404)
#define CAN_TSR       (*(volatile uint32_t *)0x40006408)
#define CAN_BTR       (*(volatile uint32_t *)0x4000641C)

/* CAN Transmit Mailbox 0 Registerları */
#define CAN_TIR       (*(volatile uint32_t *)0x40006580)
#define CAN_TDTR      (*(volatile uint32_t *)0x40006584)
#define CAN_TDLR      (*(volatile uint32_t *)0x40006588)

/* CAN Filtre Registerları */
#define CAN_FMR       (*(volatile uint32_t *)0x40006600)
#define CAN_FS1R      (*(volatile uint32_t *)0x40006604)
#define CAN_FFA1R     (*(volatile uint32_t *)0x40006608)
#define CAN_FA1R      (*(volatile uint32_t *)0x4000660C)
#define CAN_FiR1      (*(volatile uint32_t *)0x40006640)
#define CAN_FiR2      (*(volatile uint32_t *)0x40006644)

void can_init(void) {
    RCC_APB2ENR |= (1 << 2);
    RCC_APB1ENR |= (1 << 25);

    GPIOA_CRH &= ~0x000FF000;
    GPIOA_CRH |=  0x000B4000;

    CAN_MCR |= (1 << 0);
    while (!(CAN_MSR & (1 << 0)));

    CAN_MCR &= ~(1 << 1);
    CAN_BTR = 0x001C0003;

    CAN_MCR &= ~(1 << 0);
    while (CAN_MSR & (1 << 0));

    CAN_FMR |= (1 << 0);
    CAN_FA1R &= ~(1 << 0);
    CAN_FS1R |= (1 << 0);
    CAN_FiR1 = 0;
    CAN_FiR2 = 0;
    CAN_FA1R |= (1 << 0);
    CAN_FMR &= ~(1 << 0);
}

void can_send(uint32_t id, uint8_t *data, uint8_t len) {
    if (CAN_TSR & (1 << 26)) {
        CAN_TIR = (id << 21);
        CAN_TDTR = (len & 0xF);
        CAN_TDLR = (data[3]<<24) | (data[2]<<16) | (data[1]<<8) | data[0];
        CAN_TIR |= (1 << 0);
    }
}

int main(void) {
    uint8_t payload[4] = {0xDE, 0xAD, 0xBE, 0xEF};
    can_init();

    while (1) {
        can_send(0x123, payload, 4);
        for (volatile int i = 0; i < 500000; i++);
    }
}
