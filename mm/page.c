#include <sys.h>
/*
 * Following global vars are defined in mem.S
 */
extern uint32_t text_start;
extern uint32_t text_end;
extern uint32_t data_start;
extern uint32_t data_end;
extern uint32_t rodata_start;
extern uint32_t rodata_end;
extern uint32_t bss_start;
extern uint32_t bss_end;
extern uint32_t heap_start;
extern uint32_t heap_size;

/*
 * _alloc_start points to the actual start address of heap pool
 * _alloc_end points to the actual end address of heap pool
 * _num_pages holds the actual max number of pages we can allocate.
 */

static uint32_t _alloc_start = 0;
static uint32_t _alloc_end = 0;
static uint32_t _num_pages = 0;

#define PAGE_SIZE 4096
#define PAGE_ORDER 12

#define PAGE_TAKEN (uint8_t)(1)
#define PAGE_LAST (uint8_t)(1 << 1)

/*
 * Page Descriptor
 * flags:
 * - bit 0: flag if this page is taken(allocated)
 * - bit 1: flag if this page is the last page of the memory block allocated
 */

struct Page {
    uint8_t flags;
};

static inline void _init_clear_page(struct Page *page) {
    page->flags = 0;
}

static inline int _page_is_free(struct Page *page) {
    if (page->flags & PAGE_TAKEN) {
        return 0;
    } else {
        return 1;
    }
}

static inline void _set_flag(struct Page *page, uint8_t flags) {
    page->flags |= flags;
}

static inline int _is_last_page(struct Page *page) {
    if (page->flags & PAGE_LAST) {
        return 1;
    } else {
        return 0;
    }
}

/*
 * align the address to the border of page(4K)
 */

static inline uint32_t _align_page(uint32_t address)
{
	uint32_t order = (1 << PAGE_ORDER) - 1;
	return (address + order) & (~order);
}

void page_init()
{
	/* 
	 * We reserved 8 Page (8 x 4096) to hold the Page structures.
	 * It should be enough to manage at most 128 MB (8 x 4096 x 4096) 
	 */
	_num_pages = (heap_size / PAGE_SIZE) - 8;
	printf("HEAP_START = %x, HEAP_SIZE = %x, num of pages = %d\n", heap_start, heap_size, _num_pages);
	
	struct Page *page = (struct Page *)heap_start;
	for (int i = 0; i < _num_pages; i++) {
		_init_clear_page(page);
		page++;	
	}

	_alloc_start = _align_page(heap_start + 8 * PAGE_SIZE);
	_alloc_end = _alloc_start + (PAGE_SIZE * _num_pages);

	printf("TEXT:   0x%x -> 0x%x\n", text_start, text_end);
	printf("RODATA: 0x%x -> 0x%x\n", rodata_start, rodata_end);
	printf("DATA:   0x%x -> 0x%x\n", data_start, data_end);
	printf("BSS:    0x%x -> 0x%x\n", bss_start, bss_end);
	printf("HEAP:   0x%x -> 0x%x\n", _alloc_start, _alloc_end);
}

/*
 * Allocate a memory block which is composed of contiguous physical pages
 * - npages: the number of PAGE_SIZE pages to allocate
 */

void *page_alloc(int npages)
{
	/* Note we are searching the page descriptor bitmaps. */
	int found = 0;
	struct Page *page_i = (struct Page *)heap_start;
	for (int i = 0; i <= (_num_pages - npages); i++) {
		if (_page_is_free(page_i)) {
			found = 1;
			/* 
			 * meet a free page, continue to check if following
			 * (npages - 1) pages are also unallocated.
			 */
			struct Page *page_j = page_i + 1;
			for (int j = i + 1; j < (i + npages); j++) {
				if (!_page_is_free(page_j)) {
					found = 0;
					break;
				}
				page_j++;
			}
			/*
			 * get a memory block which is good enough for us,
			 * take housekeeping, then return the actual start
			 * address of the first page of this memory block
			 */
			if (found) {
				struct Page *page_k = page_i;
				for (int k = i; k < (i + npages); k++) {
					_set_flag(page_k, PAGE_TAKEN);
					page_k++;
				}
				page_k--;
				_set_flag(page_k, PAGE_LAST);
				return (void *)(_alloc_start + i * PAGE_SIZE);
			}
		}
		page_i++;
	}
	return NULL;
}

/*
 * Free the memory block
 * - p: start address of the memory block
 */
void page_free(void *p)
{
	/*
	 * Assert (TBD) if p is invalid
	 */
	if (!p || (uint32_t)p >= _alloc_end) {
		panic("trying to free an empty page!");
		return;
	}
	/* get the first page descriptor of this memory block */
	struct Page *page = (struct Page *)heap_start;
	page += ((uint32_t)p - _alloc_start)/ PAGE_SIZE;
	/* loop and clear all the page descriptors of the memory block */
	while (!_page_is_free(page)) {
		if (_is_last_page(page)) {
			_init_clear_page(page);
			break;
		} else {
			_init_clear_page(page);
			page++;;
		}
	}
}