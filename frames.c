#include "vmm.h"

extern int number_of_frames; 

uint32_t chk_free_frame()
{
	for (int i = 0; i < number_of_frames; i++) {
		if (frames[i].used == false) {
			// Frame i is free
			return i;
		}
	}
	// No free frame
	return -1;
}

uint32_t page_out(uint32_t pgnum)
{
	char buf[FRAME_SIZE];
	struct timeval min_reftime = frames[0].reftime;
	int victim = 0;

	for (int i = 1; i < number_of_frames; i++) {
		// Search lasst-recently-referenced entry to be removed
		if (frames[i].used == true && timercmp(&frames[i].reftime, &min_reftime, <)) {
			min_reftime = frames[i].reftime;
			victim = i;
		}
	}

	// Write frame number of victim in backing store
	memcpy(buf, memory[victim], FRAME_SIZE);
	fseek(bs, pgnum*FRAME_SIZE, SEEK_SET);

	if (fwrite(buf, FRAME_SIZE, 1, bs) < 1) {
		perror("fwrite");
		exit(1);
	}

	// Remove TLB entry
	del_TLB_entry(frames[victim].pgnum);
	
	// Remove frame
	memset(memory[victim], 0, FRAME_SIZE);
	PGTBL[frames[victim].pgnum].frnum = -1;
	PGTBL[pgnum].frnum = victim;
	frames[victim].pgnum = -1;
	frames[victim].used = false;

	

	return victim;
}

void page_in(uint32_t pgnum, uint32_t frnum)
{
	char buf[FRAME_SIZE];
	
	// Read frame in backing store into memory
	fseek(bs, pgnum*FRAME_SIZE, SEEK_SET);
	if (fread(buf, FRAME_SIZE, 1, bs) < 1) {
		if (ferror(bs)) {
			perror("fread");
			exit(1);
		}
		clearerr(bs);
	}
	
	memcpy(memory[frnum], buf, FRAME_SIZE);
	frames[frnum].pgnum = pgnum;
	frames[frnum].used = true;
	gettimeofday(&frames[frnum].reftime, NULL);
}
