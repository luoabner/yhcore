#ifndef __sched__
#define __sched__

#define reg_t int
typedef struct _hart_reg{
    /* reg zero is ignored, since it remains 0 all the time */
    reg_t ra;
    reg_t sp;
    reg_t gp;
    reg_t tp;
    reg_t t0;
    reg_t t1, t2;
    reg_t s0;
    reg_t s1;
    reg_t a0, a1;
    reg_t a2, a3, a4, a5, a6, a7;
    reg_t s2, s3, s4, s5, s6, s7, s8, s9, s10, s11;
    reg_t t3, t4, t5, t6;
} hart_reg;

extern void schedule_init(void);
extern void schedule(void);
#endif