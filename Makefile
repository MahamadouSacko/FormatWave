EXEC=app.exe
SOURCES=wave.c menu.c args.c main.c
OBJECTS=$(SOURCES:.c=.o)
CC=gcc
CFLAGS=-W -Wall
 
.PHONY: default clean
 
default: $(EXEC)
 
wave.o: wave.c wave.h
menu.o: menu.c menu.h
args.o: args.c args.h

main.o: main.c wave.h menu.h args.h
%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)
 
$(EXEC): $(OBJECTS)
	$(CC) -o $@ $^
 
clean:
	rm -rf $(EXEC) $(OBJECTS) $(SOURCES:.c=.c~) $(SOURCES:.c=.h~) Makefile~
