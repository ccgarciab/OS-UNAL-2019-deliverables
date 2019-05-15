vpath %.c ../src_global

#source files
SRCS = $(notdir $(wildcard ../src_global/*.c))

#target name
CLIENT = dogClient
SERVER = dogServer

#compiler, compile flags, linking flags
CC = gcc
OPTS = -I../include -Wall -O -Wno-unused-result
#LIBS =

OBJS = $(SRCS:.c=.o)
OBJ_C = p2-dogClient.o $(OBJS)
OBJ_S = p2-dogServer.o $(OBJS)

all: $(SERVER) $(CLIENT)

$(SERVER): $(OBJ_S)
	$(CC) -o $(SERVER) $(OBJ_S) $(LIBS)

$(CLIENT): $(OBJ_C)
	$(CC) -o $(CLIENT) $(OBJ_C) $(LIBS)

%.o: %.c
	$(CC) $(OPTS) -c $< -o $@

d: OPTS += -g
d: all

clean:
	rm -f $(OBJS) $(CLIENT) $(SERVER) p2-dogClient.o p2-dogServer.o
