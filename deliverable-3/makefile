vpath %.c ../src_global

#source files
SRCS = pet_input.c pet_output.c error_handle.c sock_aux.c pet_file.c
SC = $(SRCS)
SS = pet_hashtable.c $(SRCS)

SRCS_C = p3-dogClient.c $(SC)
SRCS_S = p3-dogServer.c $(SS)

#header files
HDR_C = $(addprefix ../include/, $(SC:.c=.h))
HDR_S = $(addprefix ../include/, $(SS:.c=.h))

#target name
CLIENT = dogClient
SERVER = dogServer

#compiler, compile flags, linking flags
CC = gcc
OPTS = -I../include -Wall -O -Wno-unused-result
LIBS = -lpthread

OBJ_C = $(SRCS_C:.c=.o)
OBJ_S = $(SRCS_S:.c=.o)

all: $(SERVER) $(CLIENT)
	mkdir client server &>/dev/null
	touch dataDogs.dat

program: all tidy

$(SERVER): $(OBJ_S) $(HDR_S)
	$(CC) -o $(SERVER) $(OBJ_S) $(LIBS)

$(CLIENT): $(OBJ_C) $(HDR_C)
	$(CC) -o $(CLIENT) $(OBJ_C) $(LIBS)

%.o: %.c
	$(CC) $(OPTS) -c $< -o $@

d: OPTS += -g
d: all

random: error_handle.o
	$(CC) ../generation/randomStructs.c -o randomStructs error_handle.o $(OPTS)
	cp ../generation/*.txt .

tidy:
	rm -f $(OBJ_C) $(OBJ_S) randomStructs
	rm -f *.txt

clean: tidy
	rm -f $(SERVER) $(CLIENT)
