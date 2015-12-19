.SUFFIXES:.c .o

CC=gcc
SRCS=QuickSort.c
OBJS=$(SRCS:.c=.o)
EXEC=QuickSort

start:$(OBJS)
	$(CC) -o $(EXEC) $(OBJS)
	@echo "------->   OK   <-------"
.c.o:
	$(CC) -Wall -g -o $@ -c $<
clean:
	rm -f $(OBJS)
	rm -f core*
