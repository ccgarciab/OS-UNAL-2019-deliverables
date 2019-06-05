vpath %.c ../src_global

#source files
SRCS = $(notdir $(wildcard ../src_global/*.c))
SC = p2-dogClient.c $(SRCS)
SS = p2-dogServer.c $(SRCS)

#target name
CLIENT = dogClient
SERVER = dogServer

#compiler, compile flags, linking flags
CC = gcc
OPTS = -I../include -Wall -O -Wno-unused-result
#LIBS =

OBJ_C = $(SC:.c=.o)
OBJ_S = $(SS:.c=.o)

all: $(SERVER) $(CLIENT)

program: all tidy

$(SERVER): $(OBJ_S)
	$(CC) -o $(SERVER) $(OBJ_S) $(LIBS) -lpthread

$(CLIENT): $(OBJ_C)
	$(CC) -o $(CLIENT) $(OBJ_C) $(LIBS) -lpthread

%.o: %.c
	$(CC) $(OPTS) -c $< -o $@

d: OPTS += -g
d: all

tidy:
	rm -f $(OBJ_C) $(OBJ_S)

clean: tidy
	rm -f $(SERVER) $(CLIENT)
