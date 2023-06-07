#include <sys.h>
#include <core/sched.h>

extern void init_entry(void);
extern void trap_init(void);

void kernel_entry(){
    uart_init();
    uart_puts("Hello, world!\n");
    page_init();
    trap_init();
    schedule_init();
    init_entry();
    schedule();
    uart_puts("Would not go here!\n");
    while(1){;}
}