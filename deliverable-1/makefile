vpath %.c ../src_global

#source files
SRCS = p1-dogProgram.c $(notdir $(wildcard ../src_global/*.c))

#target name
TARG = dogProgram

#compiler, compile flags, linking flags
CC = gcc
OPTS = -I../include -Wall -O -Wno-unused-result
#LIBS =

OBJS = $(SRCS:.c=.o)

program: all tidy

all: $(TARG)

$(TARG): $(OBJS)
	$(CC) -o $(TARG) $(OBJS) $(LIBS)

%.o: %.c
	$(CC) $(OPTS) -c $< -o $@

d: OPTS += -g
d: $(TARG)

tidy:
	rm -f $(OBJS)

clean: tidy
	rm -f $(TARG)
