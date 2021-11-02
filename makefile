CC		= gcc
CFLAGS		= -Wall
SRCS		= vmm.c init.c reference.c tlb.c pgtbl.c frames.c
OBJS   	= $(SRCS:.c=.o)
PROGRAM	= vmm
HEADER		= vmm.h

$(PROGRAM): $(OBJS)
	$(CC) -o $@ $^

$(OBJS): $(SRCS)
	$(CC) $(CFLAGS) -c $(SRCS)

$(OBJS): $(HEADER)

.PHONY: clean
clean:
	$(RM) $(PROGRAM) $(OBJS)


