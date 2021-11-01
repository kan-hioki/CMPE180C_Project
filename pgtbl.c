#include "vmm.h"

uint32_t search_PGTBL(uint32_t pgnum)
{
	return PGTBL[pgnum].frnum;
}

void set_PGTBL(uint32_t pgnum, uint32_t frnum)
{
	PGTBL[pgnum].frnum = frnum;
}
