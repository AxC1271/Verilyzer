CC = gcc
CFLAGS = -Wall -g
LDFLAGS = 

# Parser targets
PARSER_OBJS = main.o circuit.o lex.yy.o parse.tab.o
PARSER_TARGET = circuit_parser

# Simulator targets
SIM_OBJS = sim_main.o simulator.o
SIM_TARGET = circuit_simulator

.PHONY: all clean parser simulator

all: parser simulator

parser: $(PARSER_TARGET)

simulator: $(SIM_TARGET)

# Parser build rules
$(PARSER_TARGET): $(PARSER_OBJS)
	$(CC) $(CFLAGS) -o $(PARSER_TARGET) $(PARSER_OBJS) $(LDFLAGS)

main.o: main.c circuit.h
	$(CC) $(CFLAGS) -c main.c -o main.o

circuit.o: circuit.c circuit.h
	$(CC) $(CFLAGS) -c circuit.c -o circuit.o

lex.yy.c: tokens.l
	flex tokens.l

lex.yy.o: lex.yy.c parse.tab.h
	$(CC) $(CFLAGS) -c lex.yy.c -o lex.yy.o

parse.tab.c parse.tab.h: parse.y
	bison -d parse.y

parse.tab.o: parse.tab.c circuit.h
	$(CC) $(CFLAGS) -c parse.tab.c -o parse.tab.o

# Simulator build rules
$(SIM_TARGET): $(SIM_OBJS)
	$(CC) $(CFLAGS) -o $(SIM_TARGET) $(SIM_OBJS) $(LDFLAGS)

sim_main.o: sim_main.c simulator.h
	$(CC) $(CFLAGS) -c sim_main.c -o sim_main.o

simulator.o: simulator.c simulator.h circuit.h
	$(CC) $(CFLAGS) -c simulator.c -o simulator.o

clean:
	rm -f $(PARSER_OBJS) $(SIM_OBJS) lex.yy.c parse.tab.c parse.tab.h 
	rm -f $(PARSER_TARGET) $(SIM_TARGET) circuit_output.txt