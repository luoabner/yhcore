#ifndef __sys__
#define __sys__
#include "types.h"
#include "hardware.h"

#include <stddef.h>
#include <stdarg.h>

/* uart */
// extern void uart_init();
extern int uart_putchar(char ch);
extern void uart_puts(char *s);

/* printf */
extern int  printf(const char* s, ...);
extern void panic(char *s);

/* memory management */
// extern void page_init();
extern void *page_alloc(int npages);
extern void page_free(void *p);
#endif