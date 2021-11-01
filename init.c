#include "vmm.h"

#define BACKING_STORE	"BACKING_STORE.bin"

void init()
{
	// Initialize TLB
	for (int i = 0; i < TLB_SIZE; i++) {
		TLB[i].pgnum = -1;
		TLB[i].frnum = -1;
	}

	// Initialize page table
	for (int i = 0; i < PGTBL_SIZE; i++) {
		PGTBL[i].frnum = -1;
	}
	
	for (int i = 0; i < NUM_FRAME; i++) {
		frames[i].used = false;
	}
	
	// Create backing store
	char buf[PGTBL_SIZE*FRAME_SIZE];
	memset(buf, '\0', PGTBL_SIZE*FRAME_SIZE);
	if ((bs = fopen(BACKING_STORE, "r+b")) == NULL) {
		perror("fopen");
		exit(1);
	}
}
