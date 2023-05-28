extern void uart_init(void);
extern void uart_puts(char *s);

void kernel_entry(){
    uart_puts("Hello, world!\n");
    while(1){;}
}