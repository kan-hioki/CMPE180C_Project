#include "vmm.h"

#define MAX_DIGITS	10

int main(int argc, char *argv[])
{
	if (argc != 2) {
		fprintf(stderr, "Usage: vmm [reference_list]\n");
	}

	uint32_t vaddr, phyaddr;
	int ch;
	char buf[MAX_DIGITS+1];
	FILE *ref;
	
	// Open reference list
	if ((ref = fopen(argv[1], "r")) == NULL) {
		perror("fopen");
		exit(1);
	}
	
	// Initialization	
	init();
	TLBhit_count = 0;
	// Address translation
	int ref_count = 0;
	while (fgets(buf, sizeof(buf), ref)) {
		vaddr = (uint32_t)atoi(buf);
		phyaddr = reference(vaddr, &ch);
		printf("Virtual address: %d Physical address: %d Value: %d\n",vaddr, phyaddr, ch);
		ref_count++;
	}
	printf("\nPage fault rate: %f TLB hit rate: %f\n", (double)pfault_count/ref_count, (double)TLBhit_count/ref_count);
printf("%d\n", pfault_count);
	fclose(ref);
	fclose(bs);
	
	return 0;
}
