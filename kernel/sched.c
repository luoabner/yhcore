#include <core/sched.h>
#include <sys.h>
extern void task_switch(hart_reg* context);
extern void uart_puts(char* s);
#define STACK_SIZE 8192

#define MAX_TASK 10

uint8_t task_stack[MAX_TASK][STACK_SIZE];
hart_reg machine_tasks[MAX_TASK];

volatile static int _top = 0; /* first available context task index */
volatile static int _cur = -1; /* current task context */

static inline void w_mscratch(reg_t reg){
    asm volatile ("csrw mscratch, %0" :: "r" (reg)); /* put register reg into csrw */
}

void schedule_init(){
    w_mscratch(0);
}

/* simple FIFO scheduler */
void schedule(){
    if (_top < 0){
        panic("Task index should be larger than 0!");
        return;
    }
    _cur = (_cur + 1) % _top;
    hart_reg *next = &machine_tasks[_cur];
    task_switch(next);
}

/* 
    Create a task, which enters at task_entry
*/
int task_create(void(* task_entry)(void)){
    if (_top < MAX_TASK){
        machine_tasks[_top].sp = (reg_t) &task_stack[_top];
        machine_tasks[_top].ra = (reg_t) task_entry;
        _top++;
        return 0;
    }
    return 1;
}

void task_yield(){
    schedule();
}
volatile void task_delay(int count){
    count = count << 10;
    while(count--){;}
}