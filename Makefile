CC = gcc
CFLAGS = -Wall -Wno-unused -g
LDFLAGS = -g
LDLIBS = -lm

SRCS = main.c pboDataTypes.c pboExtractor.c utils.c
OBJS = $(patsubst %.c,%.o,$(SRCS))
BIN = pboExtractor

.PHONY:		all depend clean dist-clean

all:		$(BIN)

$(BIN):		$(OBJS)
		$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

%.o:		%.c
		$(CC) $(CFLAGS) -o $@ -c $<


-include depend.mak

depend:		
		$(CC) $(CFLAGS) -MM $(SRCS) > depend.mak

clean:
		rm -f *~ *.o

dist-clean:	clean
		rm -f $(BIN) depend.mak
