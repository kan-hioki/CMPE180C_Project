#include "vmm.h"

uint32_t search_TLB(uint32_t pgnum)
{
	for (int i = 0; i < TLB_SIZE; i++) {
		if (TLB[i].pgnum == pgnum) {
			// TLB entry found
			return TLB[i].frnum;
		}
	}
	// No TLB entry
	return -1;
}

void crt_TLB_entry(uint32_t pgnum, uint32_t frnum)
{
	for (int i = 0; i < TLB_SIZE; i++) {
		// Empty TLB slot found
		if (TLB[i].pgnum == -1) {
			TLB[i].pgnum = pgnum;
			TLB[i].frnum = frnum;
			gettimeofday(&TLB[i].reftime, NULL);
			return;
		}
	}

	// No empty TLB slot
	struct timeval min_reftime = TLB[0].reftime;
	int removed = 0;

	// Search lasst-recently-referenced entry to be removed
	for (int i = 1; i < TLB_SIZE; i++) {
		if (timercmp(&TLB[i].reftime, &min_reftime, <)) {
			min_reftime = TLB[i].reftime;
			removed = i;
		}
	}

	// Create new TLB entry at removed location
	TLB[removed].pgnum = pgnum;
	TLB[removed].frnum = frnum;
	gettimeofday(&TLB[removed].reftime, NULL);
}

void del_TLB_entry(uint32_t pgnum)
{
	for (int i = 0; i < TLB_SIZE; i++) {
		// TLB entry found
		if (TLB[i].pgnum == pgnum) {
			TLB[i].pgnum = -1;
			TLB[i].frnum = -1;
		}
	}
}
