#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "circuit.h"

// Logic value
typedef enum {
    VALUE_0 = 0,
    VALUE_1 = 1,
    VALUE_X = 2
} LogicValue;

typedef struct SimGate {
    int id;
    GateType type;
    char* name;
    int is_input;
    int is_output;
    int is_dff;
    int level;

    int* fanins;
    int fanin_count;
    int* fanouts;
    int fanout_count;

    LogicValue state;
    LogicValue next_state;
    int sched; // pointer to next gate, initialized as -1 starting (if not scheduled)
} SimGate;

// Simulator circuit struct
typedef struct Simulator {
    SimGate* gates;
    int gate_count;
    int input_count;
    int output_count;
    int dff_count;
    int max_level;

    int* input_indices;
    int* output_indices;
    int* dff_indices;

    SimGate** levels;
    int dummy_gate_id;
} Simulator;

// necessary function prototypes
void init_simulator(Simulator* sim);
void load_circuit_file(const char* filename, Simulator* sim);
void init_lookup_tables(void);

// evaluate logic value by algorithm
LogicValue evaluate_input_scan(SimGate* gate, SimGate* all_gates);
LogicValue evaluate_lookup_table(SimGate* gate, SimGate* all_gates);

// scheduling
void schedule_fanout(int gate_id, Simulator* sim);
void schedule_gate(int gate_id, Simulator* sim);

// simulation
void simulate(Simulator* sim, int use_lookup_table);
void print_state(Simulator* sim, int cycle);

// cleanup
void free_simulator(Simulator* sim);

// utility
const char* logic_value_str(LogicValue V);

#endif