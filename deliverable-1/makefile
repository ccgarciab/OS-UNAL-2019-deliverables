#source files
SRCS = p1-dogProgram.c pet_input.c pet_hashtable.c

#target name
TARG = dogProgram

#compiler, compile flags, linking flags
CC = gcc
OPTS = -Wall -O
#LIBS =

OBJS = $(SRCS:.c=.o)

$(TARG): $(OBJS)
	$(CC) -o $(TARG) $(OBJS) $(LIBS)

%.o: %.c
	$(CC) $(OPTS) -c $< -o $@

d: OPTS += -g
d: $(TARG)

clean:
	rm -f $(OBJS) $(TARG)
