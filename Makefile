# IFJ project 2018
# @author Juraj Holub <xholub40@stud.fit.vutbr.cz>

CC = gcc
CFLAGS = -I -std=c99 -g -Werror -Wall
OBJ = main.o \
	  expressions_parser.o \
	  symtable.o \
	  error_handle.o \
	  semantic_parser.o \
	  semantic_parser_td.o \
	  stack.o \
	  dynamic_string.o \
	  list.o \
	  gen_out.o \
	  scanner.o \
	  global_interface.o \
	  top_down_parser.o
EXECUTABLE = main

#vpath %.c src
#vpath %.h include

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ 

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $<

.PHONY: run doc pack clean test
run: $(EXECUTABLE)
	./$(EXECUTABLE)

pack:
	zip xholub40.zip *.c *.h Makefile

clean:
	rm -rf *.o *.out $(EXECUTABLE)
test:
	cd tests && bash test_outputs.bash
