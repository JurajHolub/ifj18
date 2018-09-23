CC = gcc
CFLAGS = -std=c99 -pg
OBJ = main.o
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
