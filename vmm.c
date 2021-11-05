#include "vmm.h"

#define MAX_DIGITS			10
#define DEFAULT_NUMBER_OF_FRAMES	256

int main(int argc, char *argv[])
{
	if (argc > 3) {
		fprintf(stderr, "Usage: ./vmm address_list [number_of_frames]\n");
		exit(1);
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
	if (argc == 2) {
		init(DEFAULT_NUMBER_OF_FRAMES);
	} else {
		init(atoi(argv[2]));
	}
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

	dealloc();
	fclose(ref);
	fclose(bs);
	
	return 0;
}
