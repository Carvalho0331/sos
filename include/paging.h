#ifndef PAGING_H
#define PAGING_H

/* Page flags */
#define PAGE_PRESENT  0x1
#define PAGE_RW       0x2
#define PAGE_USER     0x4

/* Number of entries per table/directory */
#define PAGE_ENTRIES  1024

typedef unsigned int page_entry_t;

/* Page directory (1024 entries × 4 bytes = 4 KB, 4 KB aligned) */
extern page_entry_t page_directory[PAGE_ENTRIES] __attribute__((aligned(4096)));

void paging_init(void);
void paging_map(unsigned int virt, unsigned int phys, unsigned int flags);
void paging_enable(void);

#endif
