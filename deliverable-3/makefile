vpath %.c ../src_global

#source files
SC = pet_input.c pet_output.c error_handle.c sock_aux.c pet_file.c

SRCS_C = p3-dogClient.c $(SC)

#header files
HDR_C = $(addprefix ../include/, $(SC:.c=.h))

#target name
CLIENT = dogClient

#compiler, compile flags, linking flags
CC = gcc
OPTS = -I../include -Wall -O -Wno-unused-result
LIBS = -lpthread

OBJ_C = $(SRCS_C:.c=.o)

all: $(CLIENT)
	mkdir client &>/dev/null
	touch dataDogs.dat

program: all tidy

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
	rm -f $(OBJ_C) randomStructs
	rm -f *.txt

clean: tidy
	rm -f $(CLIENT)

