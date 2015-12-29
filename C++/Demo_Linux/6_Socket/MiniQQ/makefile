.SUFFIXES: .c .o

CC=gcc
SERVERSRCS=qqserver.c\
			pub.c
CLIENTSRCS=qqclient.c
SERVEROBJS=$(SERVERSRCS:.c=.o)
CLIENTOBJS=$(CLIENTSRCS:.c=.o)
SERVEREXEC=qqserver
CLIENTEXEC=qqclient

all:$(SERVEROBJS) $(CLIENTOBJS)
	$(CC) -o $(SERVEREXEC) $(SERVEROBJS) -lpthread
	$(CC) -o $(CLIENTEXEC) $(CLIENTOBJS) -lpthread 
	@echo '-------------ok--------------'

.c.o:
	$(CC) -Wall -g -o $@ -c $< 

clean:
	rm -f $(SERVEROBJS)
	rm -f $(CLIENTOBJS)
	rm -f core*

