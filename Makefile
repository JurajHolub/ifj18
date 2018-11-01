CC = gcc
CFLAGS = -I -std=c99 -g -Werror -Wall
OBJ = test.o \
	  expressions_parser.o \
	  symtable.o \
	  error_handle.o \
	  semantic_parser.o \
	  stack.o \
	  dynamic_string.o \
	  top_down_parser.o
EXECUTABLE = main

#vpath %.c src
#vpath %.h include

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ 

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $<

.PHONY: run doc pack clean
run: $(EXECUTABLE)
	./$(EXECUTABLE)

pack:
	zip xlogin00.zip *.c *.h Makefile

clean:
	rm -rf *.o *.out $(EXECUTABLE)
