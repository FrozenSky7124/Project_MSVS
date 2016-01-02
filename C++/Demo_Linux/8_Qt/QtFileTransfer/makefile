.SUFFIXES: .c .o

CC=gcc
SERVERSRCS=server.c\
			pub.c
CLIENTSRCS=client.c\
			pub.c
		
SERVEROBJS=$(SERVERSRCS:.c=.o)
CLIENTOBJS=$(CLIENTSRCS:.c=.o)
SERVEREXEC=server
CLIENTEXEC=client

all:$(SERVEROBJS) $(CLIENTOBJS)
	$(CC) -o $(SERVEREXEC) $(SERVEROBJS)
	$(CC) -o $(CLIENTEXEC) $(CLIENTOBJS)
	@echo '-------------ok--------------'

.c.o:
	$(CC) -Wall -o $@ -c $< 

clean:
	rm -f $(SERVEROBJS)
	rm -f $(CLIENTOBJS)
	rm -f core*

