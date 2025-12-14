%{

#include <stdio.h>
#include <string.h>
#include "circuit.h"

extern int yyerror();
extern int yyparse();
extern int yylex();
extern int yylineno;

char current_gate_name[256];
GateType current_gate_type;
char *port_names[100];
int port_count = 0;

// Store DFF connections for deferred resolution
char *dff_d_inputs[100];
char *dff_q_outputs[100];
int dff_conn_count = 0;

#define YYDEBUG 1
%}

%start module

%union {
    char *id;
    unsigned long ln;
}

%token NAND WIRE AND OR NOR XOR XNOR BUF NOT DFF OUTPUT INPUT MODULE ENDMODULE 

%token <id> _NAME _STRING
%token <ln> _NUMBER
%token <ln> _BASENUMBER

%%

module: 
    MODULE _NAME '(' port_list ')' ';' module_items ENDMODULE {
        printf("\nFinished parsing module.\n");
    }
    ;

port_list: 
    port_ref
    | port_list ',' port_ref
    ;

port_ref: 
    _NAME {
        /* Port names in module declaration - we don't need to process these */
        /* They'll be defined as inputs/outputs later */
    }
    ;

module_items: 
    module_item
    | module_items module_item
    ;

module_item: 
    input_declaration
    | output_declaration
    | net_declaration
    | gate_instantiation
    | error ';'
    ;

input_declaration: 
    INPUT input_list ';' {
        printf("Input declaration processed\n");
    }
    ;

input_list:
    _NAME {
        add_gate($1, GATE_INPUT);
        int idx = circuit.gate_count - 1;
        circuit.gates[idx].output_wire = strdup($1);
    }
    | input_list ',' _NAME {
        add_gate($3, GATE_INPUT);
        int idx = circuit.gate_count - 1;
        circuit.gates[idx].output_wire = strdup($3);
    }
    ;

output_declaration: 
    OUTPUT output_list ';' {
        printf("Output declaration processed\n");
    }
    ;

output_list:
    _NAME {
        set_gate_as_output($1);
    }
    | output_list ',' _NAME {
        set_gate_as_output($3);
    }
    ;

net_declaration: 
    net_type wire_list ';'
    ;

wire_list:
    _NAME {
        /* Wire declarations - just ignore them */
    }
    | wire_list ',' _NAME {
        /* Wire declarations - just ignore them */
    }
    ;

net_type: 
    WIRE {
        printf("Wire declaration\n");
    }
    ;

gate_instantiation:
    gate_type _NAME {
        strncpy(current_gate_name, $2, 255);
        current_gate_name[255] = '\0';
        port_count = 0;
    } '(' gate_port_list ')' ';' {
        
        /* Create the gate */
        add_gate(current_gate_name, current_gate_type);
        int gate_idx = circuit.gate_count - 1;
        
        /* Store output wire name */
        if (port_count > 0) {
            if (current_gate_type == GATE_DFF) {
                // DFF: port[0] is Q (output), port[1] is D (input)
                circuit.gates[gate_idx].output_wire = strdup(port_names[0]);
                
                // Store D connection for later (after all gates are parsed)
                if (port_count >= 2) {
                    dff_d_inputs[dff_conn_count] = strdup(port_names[1]);
                    dff_q_outputs[dff_conn_count] = strdup(port_names[0]);
                    dff_conn_count++;
                }
            } else {
                // Regular gates: port[0] is output, rest are inputs
                circuit.gates[gate_idx].output_wire = strdup(port_names[0]);
                
                // Connect inputs to this gate's output
                for (int i = 1; i < port_count; i++) {
                    add_connection(port_names[i], port_names[0]);
                }
            }
        }
        
        port_count = 0;
    }
    ;

gate_port_list:
    _NAME {
        port_names[port_count] = strdup($1);  // MAKE A COPY
        port_count++;
    }
    | gate_port_list ',' _NAME {
        port_names[port_count] = strdup($3);  // MAKE A COPY
        port_count++;
    }
    ;

gate_type: 
    AND {
        current_gate_type = GATE_AND;
    }
    | NAND {
        current_gate_type = GATE_NAND;
    }
    | OR {
        current_gate_type = GATE_OR;
    }
    | NOR {
        current_gate_type = GATE_NOR;
    }
    | XOR {
        current_gate_type = GATE_XOR;
    }
    | XNOR {
        current_gate_type = GATE_XNOR;
    }
    | BUF {
        current_gate_type = GATE_BUF;
    }
    | NOT {
        current_gate_type = GATE_NOT;
    }
    | DFF {
        current_gate_type = GATE_DFF;
    }
    ;

%%

extern char *yytext;

int yyerror(const char *s) {
    printf("line %d: '%s' near '%s'\n", yylineno, s, yytext);
    return 0;
}

void resolve_dff_connections(void) {
    for (int i = 0; i < dff_conn_count; i++) {
        add_connection(dff_d_inputs[i], dff_q_outputs[i]);
        free(dff_d_inputs[i]);
        free(dff_q_outputs[i]);
    }
    dff_conn_count = 0;
}