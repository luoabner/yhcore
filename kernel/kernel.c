#include <sys.h>
void kernel_entry(){
    uart_init();
    uart_puts("Hello, world!\n");
    page_init();
    while(1){;}
}