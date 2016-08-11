.SUFFIXES: .c .o

CC=gcc
SRCS=mysql1.c\
			mydb.c
	
OBJS=$(SRCS:.c=.o)
EXEC=mysql1

all: $(OBJS)
	$(CC) -o $(EXEC) $(OBJS) -lmysqlclient
	@echo '-------------ok--------------'

.c.o:
	$(CC) -Wall -g -o $@ -c $< 

clean:
	rm -f $(OBJS)
	rm -f core*

