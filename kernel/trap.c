#include <core/sched.h>
#include <sys.h>

extern void trap_vector(void);/* see in schedule.S, line 91 */

/* Machine-mode interrupt vector */
static inline void w_mtvec(reg_t x){
	asm volatile("csrw mtvec, %0" : : "r" (x));
}

/* Machine-mode Interrupt Enable */
#define MIE_MEIE (1 << 11) // external
#define MIE_MTIE (1 << 7)  // timer
#define MIE_MSIE (1 << 3)  // software

/* read the value of mie and return */
static inline reg_t r_mie(){
	reg_t x;
	asm volatile("csrr %0, mie" : "=r" (x) );
	return x;
}

/* write value of x into mie register */
static inline void w_mie(reg_t x){
	asm volatile("csrw mie, %0" : : "r" (x));
}

/* read the value of mcause and return */
static inline reg_t r_mcause(){
	reg_t x;
	asm volatile("csrr %0, mcause" : "=r" (x) );
	return x;
}

void trap_init(){
    /*
	 * set the trap-vector base-address for machine-mode
	 */
    w_mtvec((reg_t)trap_vector); /* 将trap_vector函数的首地址放入mtvec寄存器 */
}

reg_t trap_handler(reg_t epc, reg_t cause){
    reg_t return_address = epc;
    reg_t trap_cause = cause & 0xfff;
    if (cause & 0x80000000){
        /* 是中断 */
        switch (trap_cause) {
        case 3:
            uart_puts("Machine software interrupt!\n");
            break;
        case 7:
            uart_puts("Machine timer interrupt!\n");
            break;
        case 11:
            uart_puts("Machine external interrupt!\n");
            break;
        case 42:
            uart_puts("System calls!\n");
            break;
        default:
            break;
        }
    }
    else {
        /* 是异常 */
        switch (trap_cause){
        case 0:
            uart_puts("Instruction address misaligned!\n");
            break;
        case 1:
            uart_puts("Instruction access fault!\n");
            break;
        case 2:
            uart_puts("Illegal instruction!\n");
            break;
        case 3:
            uart_puts("Breakpoint!\n");
            break;
        case 4:
            uart_puts("Load address misaligned!\n");
            break;
        case 5:
            uart_puts("Load access fault!\n");
            break;
        case 6:
            uart_puts("Store/AMO address misaligned!\n");
            break;
        case 7:
            uart_puts("Store/AMO access fault!\n");
            break;
        case 8:
            uart_puts("Environment call from U-mode!\n");
            break;
        case 9:
            uart_puts("Environment call from S-mode!\n");
            break;
        case 11:
            uart_puts("Environment call from M-mode!\n");
            break;
        case 12:
            uart_puts("Instruction page fault!\n");
            break;
        case 13:
            uart_puts("Load page fault!\n");
            break;
        case 15:
            uart_puts("Store/AMO page fault!\n");
            break;
        default:
            break;
        }
        panic("Not supported yet!");
    }
    return return_address;
}

void system_call(int syscall_number){
    // asm volatile();
	/*
	 * Synchronous exception code = 5
	 * Load access fault
	 */
	//int a = *(int *)0x00000000;

	uart_puts("Entering system call, but not implemented...\n");
}