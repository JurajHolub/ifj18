CC = gcc
CFLAGS = -I -std=c99 -g -Werror -Wall
OBJ = tests_expressions_parser.o \
	  expressions_parser.o \
	  symtable.o \
	  error_handle.o
EXECUTABLE = tests_expressions_parser

#vpath %.c ../
#vpath %.h ../

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ 

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $<

.PHONY: run doc clean
run: $(EXECUTABLE)
	./$(EXECUTABLE)

clean:
	rm -rf *.o *.out $(EXECUTABLE)
