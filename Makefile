CC = gcc
CFLAGS = -g3 -Wall -Iinclude
SRCS = main.c $(wildcard src/*.c)
OBJS = $(addprefix obj/,$(SRCS:.c=.o))
# .SUFFIXES: .c .o

# .c.o:
# 	$(CC) $(CFLAGS) -c -o obj/$@ $<

all: $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o exeFile 

# dir:
# 	@if [ ! -d obj ] ; then mkdir obj; fi