#ifndef MM_H
#define MM_H

/* Simple physical memory bitmap allocator */
#define MM_PAGE_SIZE  4096
#define MM_MEM_SIZE   (4 * 1024 * 1024)  /* 4 MB managed region */
#define MM_NUM_PAGES  (MM_MEM_SIZE / MM_PAGE_SIZE)

/* Heap allocator (placed at 2 MB, grows upwards) */
#define HEAP_START    0x200000
#define HEAP_END      0x300000  /* 1 MB heap */

void  mm_init(void);
void *mm_alloc_page(void);
void  mm_free_page(void *page);

/* Heap */
void *kmalloc(unsigned int size);
void  kfree(void *ptr);

#endif
