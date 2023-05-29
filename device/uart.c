#include "hardware.h"
#include "types.h"

#define uart_reg(reg) ((volatile uint8_t *)(UART0 + reg))

#define RHR 0	// Receive Holding Register (read mode)
#define THR 0	// Transmit Holding Register (write mode)
#define DLL 0	// LSB of Divisor Latch (write mode)
#define IER 1	// Interrupt Enable Register (write mode)
#define DLM 1	// MSB of Divisor Latch (write mode)
#define FCR 2	// FIFO Control Register (write mode)
#define ISR 2	// Interrupt Status Register (read mode)
#define LCR 3	// Line Control Register
#define MCR 4	// Modem Control Register
#define LSR 5	// Line Status Register
#define MSR 6	// Modem Status Register
#define SPR 7	// ScratchPad Register

#define LSR_RX_READY (1)
#define LSR_TX_IDLE  (1 << 5)

#define uart_read_reg(reg) (*(uart_reg(reg)))
#define uart_write_reg(reg, v) (*(uart_reg(reg)) = (v))

void uart_init(){
    uart_write_reg(IER, 0x0); // 关闭中断

    // 设置波特率
    uint8_t lcr = uart_read_reg(LCR);
	uart_write_reg(LCR, lcr | (1 << 7)); // 选中寄存器功能，控制LCR第7位为1， DLL和DLM就可以用来配置波特率
	uart_write_reg(DLL, 0x03);
	uart_write_reg(DLM, 0x00);

    // 设置奇偶校验位
    lcr = 0;
	uart_write_reg(LCR, lcr | (3 << 0)); // LCR第3位控制奇偶校验
}

int uart_putchar(char ch){
	while ((uart_read_reg(LSR) & LSR_TX_IDLE) == 0); // 轮询方式，寄存器还不空闲
	return uart_write_reg(THR, ch);
}

void uart_puts(char *s){
	while (*s) {
		uart_putchar(*s++);
	}
}