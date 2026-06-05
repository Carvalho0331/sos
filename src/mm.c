/* mm.c — Physical page allocator + simple heap allocator
 *
 * Physical allocator: bitmap over 4 MB starting at 1 MB.
 * Heap allocator:     first-fit free-list at 2 MB.
 */

#include "mm.h"

/* ------------------------------------------------------------------ */
/* Physical page bitmap                                                 */
/* ------------------------------------------------------------------ */

/* 1 bit per page; bit=1 means FREE */
static unsigned char bitmap[MM_NUM_PAGES / 8];

/* Physical base of the managed region: 1 MB */
#define PHYS_BASE 0x100000

static void bitmap_set(unsigned int idx)   { bitmap[idx / 8] |=  (1u << (idx % 8)); }
static void bitmap_clear(unsigned int idx) { bitmap[idx / 8] &= ~(1u << (idx % 8)); }
static int  bitmap_test(unsigned int idx)  { return (bitmap[idx / 8] >> (idx % 8)) & 1; }

void mm_init(void)
{
    /* Mark all pages free */
    for (unsigned int i = 0; i < MM_NUM_PAGES / 8; i++)
        bitmap[i] = 0xFF;

    /* Reserve the first 256 pages (0–1 MB) — already below PHYS_BASE, but
       we track them as "used" to prevent accidents. */
    for (unsigned int i = 0; i < 256 && i < MM_NUM_PAGES; i++)
        bitmap_clear(i);

    /* Reserve the heap region (pages covering HEAP_START..HEAP_END) */
    unsigned int heap_first = (HEAP_START - PHYS_BASE) / MM_PAGE_SIZE;
    unsigned int heap_last  = (HEAP_END   - PHYS_BASE) / MM_PAGE_SIZE;
    for (unsigned int i = heap_first; i < heap_last && i < MM_NUM_PAGES; i++)
        bitmap_clear(i);
}

void *mm_alloc_page(void)
{
    for (unsigned int i = 0; i < MM_NUM_PAGES; i++) {
        if (bitmap_test(i)) {
            bitmap_clear(i);
            return (void *)(PHYS_BASE + i * MM_PAGE_SIZE);
        }
    }
    return (void *)0; /* out of memory */
}

void mm_free_page(void *page)
{
    unsigned int addr = (unsigned int)page;
    if (addr < PHYS_BASE) return;
    unsigned int idx = (addr - PHYS_BASE) / MM_PAGE_SIZE;
    if (idx < MM_NUM_PAGES)
        bitmap_set(idx);
}

/* ------------------------------------------------------------------ */
/* Heap allocator (first-fit free list)                                 */
/* ------------------------------------------------------------------ */

/* Each block is prefixed by this header */
struct heap_block {
    unsigned int       size;   /* usable bytes (not including header) */
    int                free;   /* 1 = free, 0 = in use */
    struct heap_block *next;   /* next block in list (NULL = end)    */
};

#define HDR  sizeof(struct heap_block)

static struct heap_block *heap_head = (void *)0;
static int heap_ready = 0;

static void heap_init(void)
{
    heap_head       = (struct heap_block *)HEAP_START;
    heap_head->size = HEAP_END - HEAP_START - HDR;
    heap_head->free = 1;
    heap_head->next = (void *)0;
    heap_ready = 1;
}

void *kmalloc(unsigned int size)
{
    if (!heap_ready) heap_init();
    if (size == 0) return (void *)0;

    /* Align to 4 bytes */
    size = (size + 3) & ~3u;

    struct heap_block *b = heap_head;
    while (b) {
        if (b->free && b->size >= size) {
            /* Split if there is room for another header + at least 4 bytes */
            if (b->size > size + HDR + 4) {
                struct heap_block *nb =
                    (struct heap_block *)((char *)b + HDR + size);
                nb->size = b->size - size - HDR;
                nb->free = 1;
                nb->next = b->next;
                b->next  = nb;
                b->size  = size;
            }
            b->free = 0;
            return (void *)((char *)b + HDR);
        }
        b = b->next;
    }
    return (void *)0; /* out of heap */
}

void kfree(void *ptr)
{
    if (!ptr) return;
    struct heap_block *b = (struct heap_block *)((char *)ptr - HDR);
    b->free = 1;

    /* Merge with next block if it is also free (coalescing) */
    while (b->next && b->next->free) {
        b->size = b->size + HDR + b->next->size;
        b->next = b->next->next;
    }
}
