#include <core/sched.h>
#include <types.h>
extern void task_switch(hart_reg* context);
extern void uart_puts(char* s);
#define STACK_SIZE 8192

uint8_t task_stack[STACK_SIZE];

static inline void w_mscratch(reg_t reg){
    asm volatile ("csrw mscratch, zero" :: "r" (reg));
}

hart_reg saved_ctx;

void user_task0(void);
void schedule_init(){
    w_mscratch(0);
    saved_ctx.sp = (reg_t) &task_stack[STACK_SIZE - 1];
    saved_ctx.ra = (reg_t) user_task0;
}

void schedule(){
    hart_reg *next = &saved_ctx;
    task_switch(next);
}

volatile void task_delay(int count){
    count = count << 10;
    while(count--){;}
}

void user_task0(void){
    uart_puts("Task 0: created!\n");
    while(1){
        uart_puts("Task 0: running\n");
        task_delay(100000);
    }
}