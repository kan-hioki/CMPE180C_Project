#include "vmm.h"

#define PAGE_MASK		0x0000FF00
#define OFFSET_MASK		0x000000FF
#define OFFSET_BIT_RANGE 	8

extern int pfault_count;
extern int TLBhit_count;

uint32_t reference(uint32_t vaddr, int *ch)
{
	uint32_t pgnum = (vaddr & PAGE_MASK) >> OFFSET_BIT_RANGE;
	uint32_t offset = vaddr & OFFSET_MASK;
	uint32_t frnum;
	uint32_t phyaddr;

	if ((frnum = search_TLB(pgnum)) == -1) {
		// No TLB hit
		if ((frnum = search_PGTBL(pgnum)) == -1) {
			// Page fault
			pfault_count++;
			if ((frnum = chk_free_frame()) == -1) {
				// No available frame, page out victim frame
				frnum = page_out(pgnum);
			}
			// Referenced page is in backing store, page it in
			page_in(pgnum, frnum);
			set_PGTBL(pgnum, frnum);
		}
		// Create TLB entry
		crt_TLB_entry(pgnum, frnum);
	} else {

		TLBhit_count++;
	}
	*ch = memory[frnum][offset];
	phyaddr = (frnum << OFFSET_BIT_RANGE) + offset;

	return phyaddr;
}

