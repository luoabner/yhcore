#include <core/sched.h>
#include <sys.h>

#define DELAY 100000

void user_task0(void){
    uart_puts("Hello world from task 0!\n");
    while(1){
        uart_puts("Task 0 is running!\n");
        task_delay(DELAY);
        task_yield();
    }
}

void user_task1(void){
    uart_puts("Hello world from task 1!\n");
    while(1){
        uart_puts("Task 1 is running!\n");
        task_delay(DELAY);
        task_yield();
    }
}

void init_entry(){
    task_create(user_task0);
    task_create(user_task1);
}