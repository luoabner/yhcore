    .equ STACK_SIZE, 1024
    .equ MAX_CPU_NUMBER, 8
    .global _start
    .text
_start: # 上电后由bootloader跳转到0x80000000
    csrr t0, mhartid # 从mhartid寄存器读出hart id，然后送到t0寄存器
    mv tp, t0 # tp存放线程变量空间指针
    bnez t0, park # 如果t0不等于0，那么跳转到park(让剩下的7个hart空转)
    # 建立栈
    la a0, _bss_start
    la a1, _bss_end
    bgeu a0, a1, 2f
1:
    sw zero, (a0)
    addi a0, a0, 4
    bltu a0, a1, 1b
2:
    slli t0, t0, 10
    la sp, stacks + STACK_SIZE # 将栈设置为stacks的地址加上栈的大小
    add sp, sp, t0 # 将t0左移10位之后的值与sp相减，得到实际上我们希望的栈的位置（第0个hart）
    j kernel_entry
park:
    wfi # hart进入休眠状态，直到接收到中断信号
    j park # 死循环

stacks:
    .skip STACK_SIZE* MAX_CPU_NUMBER # 用0来填充所有的栈上的空间
    .end