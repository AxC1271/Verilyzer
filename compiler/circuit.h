#ifndef CIRCUIT_H
#define CIRCUIT_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef enum {
    GATE_INPUT = 0,
    GATE_OUTPUT = 1,
    GATE_AND = 2,
    GATE_NAND = 3,
    GATE_OR = 4,
    GATE_NOR = 5,
    GATE_XOR = 6,
    GATE_XNOR = 7,
    GATE_BUF = 8,
    GATE_NOT = 9,
    GATE_DFF = 10,
    GATE_WIRE = 11
} GateType;

// struct to hold a list of integers (for fanins/fanouts)
typedef struct IntList {
    int *items;
    int count;
    int capacity;
} IntList;

// struct for a gate
typedef struct Gate {
    int id;
    char *name;              
    char *output_wire;      
    GateType type;
    int is_output;
    int level;
    IntList fanins;
    IntList fanouts;
} Gate;

// circuit struct
typedef struct Circuit {
    Gate *gates;
    int gate_count;
    int gate_capacity;
    int input_count;
    int output_count;
    int dff_count;
} Circuit;

// global circuit
extern Circuit circuit;

// function prototypes
void init_circuit(void);
void add_gate(const char *name, GateType type);
int find_gate_by_name(const char *name);
void add_connection(const char *from_name, const char *to_name);
void set_gate_as_output(const char *name);
void assign_levels(void);
void insert_buffers(void);
void print_circuit(void);
void free_circuit(void);
void resolve_dff_connections(void);

// IntList helper functions
void init_intlist(IntList *list);
void add_to_intlist(IntList *list, int value);
void free_intlist(IntList *list);

void connect_gates_to_output_wires(void);

#endif
