CC=gcc
CFLAGS=-c -g -Wall -std=c99
LDFLAGS= -lGL -lGLU -lglut
SOURCES=main.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=StlReader

all: $(SOURCES) $(EXECUTABLE)
    
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@