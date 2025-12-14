#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "simulator.h"

void print_usage(const char* prog_name) {
    printf("Usage: %s <circuit_file> [method]|n", prog_name);
    printf("  circuit_file: Path to circuit description file (e.g., circuit_output.txt)\n");
    printf("  method: 'scan' for input scanning (default), 'table' for table lookup\n");
    printf(" %s circuit_output.txt\n", prog_name);
    printf(" %s citcuit_output.txt table\n", prog_name);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        print_usage(argv[0]);
        return 1;
    }

    const char* circuit_file = argv[1];
    int use_lookup_table = 0;

    if (argc >= 3) {
        if (strcmp(argv[2], "table") == 0) {
            use_lookup_table = 1;
            printf("Use ooga booga table method.\n");
        } else if (strcmp(argv[2], "scan") == 0) {
            use_lookup_table = 0;
            printf("Use ooga wooga scan method.\n");
        } else {
            printf("Error.\n");
        }
    } else {
        printf("Using input scanning method.\n");
    }

    // create simulator object
    Simulator sim;
    init_simulator(&sim);

    // create lookup tables
    init_lookup_tables();
    
    // load the circuit
    load_circuit_file(circuit_file, &sim);

    // run simulation
    printf("\nStarting Simulation: \n");
    printf("Enter input values as a string (for example '0110')\n");
    printf("Enter q to quit:\n");

    simulate(&sim, use_lookup_table);

    free_simulator(&sim);

    return 0;
}