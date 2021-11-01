#ifndef VMM_H_
#define VMM_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#define TLB_SIZE	16
#define PGTBL_SIZE	256
#define NUM_FRAME	256
#define FRAME_SIZE	256

void init();
uint32_t reference(uint32_t vaddr, int *ch);
uint32_t search_TLB(uint32_t pgnum);
void crt_TLB_entry(uint32_t pgnum, uint32_t frnum);
void del_TLB_entry(uint32_t pgnum);
uint32_t search_PGTBL(uint32_t pgnum);
void set_PGTBL(uint32_t pgnum, uint32_t frnum);
uint32_t chk_free_frame();
void read_into_frame(uint32_t pgnum, uint32_t frnum);
uint32_t page_out(uint32_t pgnum);
void page_in(uint32_t pgnum, uint32_t frnum);

/* Pretend to be physical memory */
char memory[PGTBL_SIZE][FRAME_SIZE];

/* TLB management*/
struct TLBentry {
	uint32_t pgnum;		// Page number
	uint32_t frnum;		// Frame number
	struct timeval reftime;	// Last referenced time
} TLB[TLB_SIZE];

/* Page table management */
struct PGTBLentry {
	uint32_t frnum;		// Frame numberore
} PGTBL[PGTBL_SIZE];

/* Frame management */
struct frame {
	uint32_t pgnum;		// Page number bound to this frame
	bool used;			// Validation flag
	struct timeval reftime;	// Last referenced time
} frames[NUM_FRAME];

FILE *bs;	// Backing store

int pfault_count;
int TLBhit_count;

#endif
