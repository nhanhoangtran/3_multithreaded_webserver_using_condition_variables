
# ifeq ($(CC),)
# ifeq ($(origin CC),undefined)
# #     CC := clang
# 	@echo "aaaa"
# endif


CC = clang
CFLAGS = -Wall -g -pthread
# CC      = mips-linux-gnu-gcc
# CFLAGS  = $(INCLUDES) -g -Wall -Wextra -D_FILE_OFFSET_BITS=64 -Wa,-mips32r2 -Wa,-mips32r2 -DWITH_THREADS -fPIC -EL -pthread
BINS = server
OBJS = server.o myqueue.o

all: $(BINS)

server: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^

clean:
	rm -rf *.dSYM $(BINS) $(OBJS)