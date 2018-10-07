CC = gcc
CFLAGS = -I -std=c99 -g -Werror -Wall
OBJ = main.o \
	  expressions_parser.o \
	  stack.o \
	  scanner.o \
	  symtable.o
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
