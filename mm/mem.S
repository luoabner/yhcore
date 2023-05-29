.section .rodata
.global heap_start
heap_start: .word _heap_start

.global heap_size
heap_size: .word _heap_size

.global text_start
text_start: .word _text_start

.global text_end
text_end: .word _text_end

.global data_start
data_start: .word _data_start

.global data_end
data_end: .word _data_end

.global rodata_start
rodata_start: .word _rodata_start

.global rodata_end
rodata_end: .word _rodata_end

.global bss_start
bss_start: .word _bss_start

.global bss_end
bss_end: .word _bss_end