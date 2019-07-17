vpath %.c ../../src_global

#source files
SS = pet_hashtable.c pet_input.c pet_output.c error_handle.c sock_aux.c pet_file.c

SRCS_S = p3-dogServer.c $(SS)

#header files
HDR_S = $(addprefix ../../include/, $(SS:.c=.h))

#target name
SERVER = dogServer

#compiler, compile flags, linking flags
CC = gcc
OPTS = -I../../include -Wall -O -Wno-unused-result
LIBS = -lpthread

OBJ_S = $(SRCS_S:.c=.o)

all: $(SERVER)
	mkdir server &>/dev/null

program: all tidy

$(SERVER): $(OBJ_S) $(HDR_S)
	$(CC) -o $(SERVER) $(OBJ_S) $(LIBS)

%.o: %.c
	$(CC) $(OPTS) -c $< -o $@

d: OPTS += -g
d: all

tidy:
	rm -f $(OBJ_S)
	rm -f *.txt

clean: tidy
	rm -f $(SERVER) 
