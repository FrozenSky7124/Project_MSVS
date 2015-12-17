.SUFFIXES:.c .o

CC=gcc
SRCS=*.c
OBJS=$(SRCS:.c=.o)
EXEC=main

start:$(OBJS)
	$(CC) -o $(EXEC) $(OBJS)
	@echo "^_^   ^_^   ^_^   ^_^   ^_^"
.c.o:
	$(CC) -Wall -g -o $@ -c $<
clean:
	rm -f $(OBJS)
	rm -f core*
