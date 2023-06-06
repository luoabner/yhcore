CROSS_COMPILE = riscv64-unknown-elf-
CFLAGS = -nostdlib -fno-builtin -march=rv32ima -mabi=ilp32 -g -Wall
LIBRARY_PATH = -I include
QEMU = qemu-system-riscv32
QFLAGS = -nographic -smp 1 -machine virt -bios none

GDB = gdb-multiarch
CC = ${CROSS_COMPILE}gcc
OBJCOPY = ${CROSS_COMPILE}objcopy
OBJDUMP = ${CROSS_COMPILE}objdump

ASM_SRC = boot/start.S \
	mm/mem.S \
	kernel/schedule.S
C_SRC = kernel/kernel.c \
	kernel/printf.c \
	kernel/sched.c \
	kernel/trap.c \
	device/uart.c \
	mm/page.c \
	init/init_entry.c 
OBJS = $(ASM_SRC:.S=.o)
OBJS += $(C_SRC:.c=.o)
# WORKING_DIRECTORY = $(shell pwd)
# DIRS := . $(shell find $(WORKING_DIRECTORY) -type d)

.DEFAULT_GOAL := all
all: os.elf

# start.o must be the first in dependency!

os.elf: ${OBJS}
	${CC} ${CFLAGS} -T build.ld -o os.elf $^
	${OBJCOPY} -O binary os.elf os.bin

%.o : %.c
	${CC} ${CFLAGS} ${LIBRARY_PATH} -c -o $@ $<

%.o : %.S
	${CC} ${CFLAGS} ${LIBRARY_PATH} -c -o $@ $<

run: all
	@${QEMU} -M ? | grep virt >/dev/null || exit
	@echo "Press Ctrl-A and then X to exit QEMU"
	@echo "------------------------------------"
	@${QEMU} ${QFLAGS} -kernel os.elf

.PHONY : debug
debug: all
	@echo "Press Ctrl-C and then input 'quit' to exit GDB and QEMU"
	@echo "-------------------------------------------------------"
	@${QEMU} ${QFLAGS} -kernel os.elf -s -S &
	@${GDB} os.elf -q -x ../gdbinit

.PHONY : code
code: all
	@${OBJDUMP} -S os.elf | less

.PHONY : clean
clean:
	rm -rf *.o *.bin *.elf
	rm -f boot/*.o device/*.o init/*.o kernel/*.o mm/*.o