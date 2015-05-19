CC=gcc
CFLAGS=-c -g -Wall -std=c99
LDFLAGS= -lGL -lGLU -lglut
SOURCES=main.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=StlReader

all: $(SOURCES) $(EXECUTABLE)
    
$(EXECUTABLE): $(OBJECTS) 
	@echo "Generating executable from object files"
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	@echo "Generating object files"
	$(CC) $(CFLAGS) $< -o $@
	
clean:
	@echo "Removing compiling files"
	rm -rf *.o
	rm -rf $(EXECUTABLE)