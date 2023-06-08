.section .rodata
.global heap_start
heap_start: .dword _heap_start

.global heap_size
heap_size: .dword _heap_size

.global text_start
text_start: .dword _text_start

.global text_end
text_end: .dword _text_end

.global data_start
data_start: .dword _data_start

.global data_end
data_end: .dword _data_end

.global rodata_start
rodata_start: .dword _rodata_start

.global rodata_end
rodata_end: .dword _rodata_end

.global bss_start
bss_start: .dword _bss_start

.global bss_end
bss_end: .dword _bss_end