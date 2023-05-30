#include <sys.h>
#include <core/sched.h>
void kernel_entry(){
    uart_init();
    uart_puts("Hello, world!\n");
    page_init();
    schedule_init();
    schedule();
    uart_puts("Would not go here!\n");
    while(1){;}
}