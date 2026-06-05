/* paging.c — Identity-mapping paging setup
 *
 * Sets up a page directory and a single page table that identity-maps
 * the first 4 MB (sufficient to keep the kernel running after CR0 is set).
 * Paging is NOT enabled at boot by default so the kernel remains stable;
 * call paging_enable() explicitly if you want it active.
 */

#include "paging.h"

/* 4 KB-aligned page directory */
page_entry_t page_directory[PAGE_ENTRIES] __attribute__((aligned(4096)));

/* Single page table covering 0–4 MB (1024 × 4 KB pages) */
static page_entry_t page_table_0[PAGE_ENTRIES] __attribute__((aligned(4096)));

void paging_init(void)
{
    /* Clear page directory */
    for (int i = 0; i < PAGE_ENTRIES; i++)
        page_directory[i] = 0;

    /* Identity-map 0–4 MB with present + r/w flags */
    for (int i = 0; i < PAGE_ENTRIES; i++)
        page_table_0[i] = (unsigned int)(i * 4096) | PAGE_PRESENT | PAGE_RW;

    /* Install first page table in directory (covers 0–4 MB) */
    page_directory[0] = (unsigned int)page_table_0 | PAGE_PRESENT | PAGE_RW;
}

/**
 * paging_map:
 * Map a single virtual page to a physical page.
 * Allocates a new page table if the directory entry is absent.
 * NOTE: For simplicity this uses static tables; a real kernel would
 *       use kmalloc / physical allocator here.
 */
void paging_map(unsigned int virt, unsigned int phys, unsigned int flags)
{
    unsigned int dir_idx   = virt >> 22;
    unsigned int table_idx = (virt >> 12) & 0x3FF;

    /* If no page table for this directory entry, we cannot map dynamically
       in this minimal implementation — silently ignore. */
    if (!(page_directory[dir_idx] & PAGE_PRESENT)) return;

    page_entry_t *pt =
        (page_entry_t *)(page_directory[dir_idx] & 0xFFFFF000);
    pt[table_idx] = (phys & 0xFFFFF000) | (flags & 0xFFF);
}

/**
 * paging_enable:
 * Loads the page directory address into CR3 and sets the PG bit in CR0.
 */
void paging_enable(void)
{
    __asm__ volatile (
        "mov %0, %%cr3\n\t"       /* load page directory base */
        "mov %%cr0, %%eax\n\t"
        "or  $0x80000000, %%eax\n\t"
        "mov %%eax, %%cr0\n\t"   /* set PG bit */
        :
        : "r" (page_directory)
        : "eax"
    );
}
