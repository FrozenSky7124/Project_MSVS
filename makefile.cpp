.SUFFIXES:.cpp .o

CC=g++
SRCS=*.cpp
OBJS=$(SRCS:.cpp=.o)
EXEC=main

start:$(OBJS)
	$(CC) -o $(EXEC) $(OBJS)
	@echo "^_^   ^_^   ^_^   ^_^   ^_^"
.cpp.o:
	$(CC) -Wall -g -o $@ -c $<
clean:
	rm -f $(OBJS)
	rm -f core*
