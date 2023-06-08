# hart_reg* base = &saved_ctx;
# base->ra = ra;

.macro reg_save base
    sd ra, 0(\base)
    sd sp, 8(\base)
    sd gp, 16(\base)
    sd tp, 24(\base)
    sd t0, 32(\base)
    sd t1, 40(\base)
    sd t2, 48(\base)
    sd s0, 56(\base)
    sd s1, 64(\base)
    sd a0, 72(\base)
    sd a1, 80(\base)
    sd a2, 88(\base)
    sd a3, 96(\base)
    sd a4, 104(\base)
    sd a5, 112(\base)
    sd a6, 120(\base)
    sd a7, 128(\base)
    sd s2, 136(\base)
    sd s3, 144(\base)
    sd s4, 152(\base)
    sd s5, 160(\base)
    sd s6, 168(\base)
    sd s7, 176(\base)
    sd s8, 184(\base)
    sd s9, 192(\base)
    sd s10, 200(\base)
    sd s11, 208(\base)
    sd t3, 216(\base)
    sd t4, 224(\base)
    sd t5, 232(\base)
.endm

# 从内存中把数都放回寄存器
# hart_reg* base = &saved_ctx;
# ra = base->ra
.macro reg_restore base
	ld ra, 0(\base)
	ld sp, 8(\base)
	ld gp, 16(\base)
	ld tp, 24(\base)
	ld t0, 32(\base)
	ld t1, 40(\base)
	ld t2, 48(\base)
	ld s0, 56(\base)
	ld s1, 64(\base)
	ld a0, 72(\base)
	ld a1, 80(\base)
	ld a2, 88(\base)
	ld a3, 96(\base)
	ld a4, 104(\base)
	ld a5, 112(\base)
	ld a6, 120(\base)
	ld a7, 128(\base)
	ld s2, 136(\base)
	ld s3, 144(\base)
	ld s4, 152(\base)
	ld s5, 160(\base)
	ld s6, 168(\base)
	ld s7, 176(\base)
	ld s8, 184(\base)
	ld s9, 192(\base)
	ld s10, 200(\base)
	ld s11, 208(\base)
	ld t3, 216(\base)
	ld t4, 224(\base)
	ld t5, 232(\base)
	ld t6, 240(\base)
.endm

.global task_switch

task_switch:
    csrrw t6, mscratch, t6 # 交换t6和mscratch
    beqz t6, 1f # 当task_switch第一次被调用时，mscratch将被设置为0
    reg_save t6 # 将除t6以外的寄存器都保存到t6中
    mv t5, t6
    csrr t6, mscratch # 将mscratch的值(t6先前的实际值)读到t6中
    sw t6, 240(t5) # 之前已经保存了30个寄存器了，每个寄存器4字节，一共偏移量为120字节

1:
    csrw mscratch, a0
    mv t6, a0
    reg_restore t6
    ret


.global trap_vector
.align 4
trap_vector:
    csrrw t6, mscratch, t6 # 交换t6和mscratch
    reg_save t6 # 以t6为base，保存除t6之外的寄存器
    mv t5, t6 # 把t6送到t5
    csrr t6, mscratch # 把t6从mscratch中又读回来
    sw t6, 240(t5) # 将t6保存到内存中
    csrw mscratch, t5 # 将t5放回mscratch，就当什么也没发生过:)

    # call trap_handler (see in trap.c)
    csrr a0, mepc
    csrr a1, mcause
    call trap_handler

    # trap_handler return the return address to a0
    csrw mepc, a0
    csrr t6, mscratch # read mscratch
    reg_restore t6

    # using mret to return to where we come from
    mret

.end