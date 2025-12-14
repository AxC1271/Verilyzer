#include "circuit.h"

Circuit circuit;

void init_intlist(IntList *list) {
    list->items = NULL;
    list->count = 0;
    list->capacity = 0;
}

void add_to_intlist(IntList *list, int value) {
    if (list->count >= list->capacity) {
        list->capacity = (list->capacity == 0) ? 4 : list->capacity * 2;
        list->items = (int *)realloc(list->items, list->capacity * sizeof(int));
        if (!list->items) {
            exit(1);
        }
    }
    list->items[list->count++] = value;
}

void free_intlist(IntList *list) {
    if (list->items) {
        free(list->items);
        list->items = NULL;
    }
    list->count = 0;
    list->capacity = 0;
}

void init_circuit(void) {
    circuit.gates = NULL;
    circuit.gate_count = 0;
    circuit.gate_capacity = 0;
    circuit.input_count = 0;
    circuit.output_count = 0;
    circuit.dff_count = 0;
}

void add_gate(const char *name, GateType type) {
    if (!name) {
        return;
    }
    int idx = find_gate_by_name(name);
    if (idx >= 0) {
        return; 
    }

    if (circuit.gate_count >= circuit.gate_capacity) {
        circuit.gate_capacity = (circuit.gate_capacity == 0) ? 16 : circuit.gate_capacity * 2;
        circuit.gates = (Gate *)realloc(circuit.gates, circuit.gate_capacity * sizeof(Gate));
        if (!circuit.gates) {
            exit(1);
        }
    }

    Gate *g = &circuit.gates[circuit.gate_count];
    g->id = circuit.gate_count;
    g->name = strdup(name);
    g->output_wire = NULL;

    if (!g->name) {
        exit(1);
    }
    g->type = type;
    g->is_output = 0;
    g->level = -1; 
    init_intlist(&g->fanins);
    init_intlist(&g->fanouts);

    if (type == GATE_INPUT) {
        circuit.input_count++;
        g->level = 0; 
    } else if (type == GATE_DFF) {
        circuit.dff_count++;
        g->level = 0; 
    }
    circuit.gate_count++;
}

int find_gate_by_name(const char *name) {
    for (int i = 0; i < circuit.gate_count; i++) {
        if (strcmp(circuit.gates[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

int find_gate_by_output_wire(const char *wire_name) {
    for (int i = 0; i < circuit.gate_count; i++) {
        if (circuit.gates[i].output_wire && 
            strcmp(circuit.gates[i].output_wire, wire_name) == 0) {
            return i;
        }
    }
    return -1;
}

void add_connection(const char *from_wire, const char *to_wire) {
    int from_id = find_gate_by_output_wire(from_wire);
    
    int to_id = find_gate_by_output_wire(to_wire);

    if (from_id < 0) {
        from_id = find_gate_by_name(from_wire);
    }
    
    if (from_id < 0 || to_id < 0) {
        fprintf(stderr, "Warning: Connection failed %s -> %s\n", from_wire, to_wire);
        return;
    }
    
    add_to_intlist(&circuit.gates[from_id].fanouts, to_id);
    add_to_intlist(&circuit.gates[to_id].fanins, from_id);
}

void set_gate_as_output(const char *name) {
    int idx = find_gate_by_name(name);
    
    if (idx < 0) {
        add_gate(name, GATE_WIRE);
        idx = circuit.gate_count - 1;
    }
    
    if (!circuit.gates[idx].is_output) {
        circuit.gates[idx].is_output = 1;
        circuit.output_count++;
    }
}

void assign_levels(void) {
    int changed = 1;
    int iteration = 0;
    
    while (changed) {
        changed = 0;
        iteration++;
        
        for (int i = 0; i < circuit.gate_count; i++) {
            Gate *g = &circuit.gates[i];
            
            if (g->level == 0) continue;

            int can_assign = 1;
            int max_level = -1;
            
            for (int j = 0; j < g->fanins.count; j++) {
                int fanin_id = g->fanins.items[j];
                int fanin_level = circuit.gates[fanin_id].level;
                
                if (fanin_level < 0) {
                    can_assign = 0;
                    break;
                }
                
                if (fanin_level > max_level) {
                    max_level = fanin_level;
                }
            }
            
            if (can_assign && max_level >= 0) {
                int new_level = max_level + 1;
                if (g->level != new_level) {
                    g->level = new_level;
                    changed = 1;
                }
            }
        }
    }
}

void insert_buffers(void) {
    int original_count = circuit.gate_count;
    int buffers_added = 0;
    
    for (int i = 0; i < original_count; i++) {
        Gate *g = &circuit.gates[i];
        
        if (g->fanouts.count > 1) {
            IntList original_fanouts;
            init_intlist(&original_fanouts);
            for (int j = 0; j < g->fanouts.count; j++) {
                add_to_intlist(&original_fanouts, g->fanouts.items[j]);
            }
            
            free_intlist(&g->fanouts);
            init_intlist(&g->fanouts);
            
            for (int j = 0; j < original_fanouts.count; j++) {
                char buf_name[256];
                sprintf(buf_name, "%s_buf%d", g->name, j);
                
                add_gate(buf_name, GATE_BUF);
                int buf_id = circuit.gate_count - 1;
                
                add_to_intlist(&g->fanouts, buf_id);
                add_to_intlist(&circuit.gates[buf_id].fanins, i);
                
                int fanout_id = original_fanouts.items[j];
                Gate *fanout = &circuit.gates[fanout_id];
                
                for (int k = 0; k < fanout->fanins.count; k++) {
                    if (fanout->fanins.items[k] == i) {
                        fanout->fanins.items[k] = buf_id;
                        break;
                    }
                }
                
                add_to_intlist(&circuit.gates[buf_id].fanouts, fanout_id);
                buffers_added++;
            }
            
            free_intlist(&original_fanouts);
        }
    }
}

void print_circuit(void) {
    int max_level = 0;
    for (int i = 0; i < circuit.gate_count; i++) {
        if (circuit.gates[i].level > max_level) {
            max_level = circuit.gates[i].level;
        }
    }
    
    printf("\nGate Levels\n");
    for (int level = 0; level <= max_level; level++) {
        int count = 0;
        for (int i = 0; i < circuit.gate_count; i++) {
            if (circuit.gates[i].level == level) {
                count++;
            }
        }
        printf("Level %d: %d gates\n", level, count);
    }
    
    printf("\nCircuit Description\n");
    printf("%d %d %d %d\n", circuit.gate_count, circuit.input_count, 
            circuit.output_count, circuit.dff_count);
    
    for (int i = 0; i < circuit.gate_count; i++) {
        Gate *g = &circuit.gates[i];
        
        printf("%d %d %d %d", (int)g->type, g->is_output, g->level, g->fanins.count);
        
        for (int j = 0; j < g->fanins.count; j++) {
            printf(" %d", g->fanins.items[j]);
        }
        
        printf(" %d", g->fanouts.count);
        for (int j = 0; j < g->fanouts.count; j++) {
            printf(" %d", g->fanouts.items[j]);
        }
        
        printf(" %s\n", g->name);
    }
    
    FILE *fp = fopen("circuit_output.txt", "w");
    if (!fp) {
        return;
    }

    fprintf(fp, "%d %d %d %d\n", circuit.gate_count, circuit.input_count, 
            circuit.output_count, circuit.dff_count);
    
    for (int i = 0; i < circuit.gate_count; i++) {
        Gate *g = &circuit.gates[i];
        
        fprintf(fp, "%d %d %d %d", (int)g->type, g->is_output, g->level, g->fanins.count);
        
        for (int j = 0; j < g->fanins.count; j++) {
            fprintf(fp, " %d", g->fanins.items[j]);
        }
        
        fprintf(fp, " %d", g->fanouts.count);
        for (int j = 0; j < g->fanouts.count; j++) {
            fprintf(fp, " %d", g->fanouts.items[j]);
        }
        
        fprintf(fp, " %s\n", g->name);
    }
    
    fclose(fp);
    printf("\nCircuit description saved to circuit_output.txt\n");
}

void free_circuit(void) {
    for (int i = 0; i < circuit.gate_count; i++) {
        free(circuit.gates[i].name);
        if (circuit.gates[i].output_wire) { 
            free(circuit.gates[i].output_wire);
        }
        free_intlist(&circuit.gates[i].fanins);
        free_intlist(&circuit.gates[i].fanouts);
    }
    
    if (circuit.gates) {
        free(circuit.gates);
    }
    
    init_circuit();
}