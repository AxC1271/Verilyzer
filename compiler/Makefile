CC = gcc
CFLAGS = -Wall -g
YACC = bison
LEX = flex

TARGET = circuit_parser

SOURCES = main.c circuit.c lex.yy.c parse.tab.c
OBJECTS = $(SOURCES:.c=.o)

HEADERS = circuit.h parse.tab.h

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)

parse.tab.c parse.tab.h: parse.y
	$(YACC) -d parse.y

lex.yy.c: tokens.l parse.tab.h
	$(LEX) tokens.l

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f main.o circuit.o lex.yy.o parse.tab.o lex.yy.c parse.tab.c parse.tab.h circuit_parser circuit_output.txt

test: $(TARGET)
	./$(TARGET) s27.v

.PHONY: all clean test