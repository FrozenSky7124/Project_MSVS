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
	# 参数-static 启动静态编译，将库文件写入可执行程序
	# 参数-lWs2_32 链接Windows库文件:Ws2_32.lib 
	$(CC) -o $(SERVEREXEC) $(SERVEROBJS)
	$(CC) -o $(CLIENTEXEC) $(CLIENTOBJS)
	@echo '^_^   ^_^   ^_^   ^_^   ^_^'

.c.o:
	# 参数-DWIN 定义WIN宏（#define WIN）用于预编译处理
	$(CC) -Wall -o $@ -c $<

clean:
	rm -f $(SERVEROBJS)
	rm -f $(CLIENTOBJS)
	rm -f core*