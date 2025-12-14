# Verilyzer

## Problem Statement

### Part 1: Compiler

<p align="center">
    <img src="./ProblemStatement.png" />
</p>

The ISCAS benchmark circuit description, shown in Figure P1(b), consists of a set of lines. Each line describes how one logic gate is interconnected with other gates. For example, the line 'dff1 XG1 DFF(G5, G10)' indicates the presence of a D-type flip-flop with output G5 (connected to the state line Q) and input G10 (connected to D).
Write a computer program (in C or C++) that reads in the circuit description as shown in Figure P2. For each gate with more than one fanout, insert BUF gates so that each line can be uniquely identified.
For every gate in the circuit, we associate a 'level'. The gate 'level' indicates the distance of that gate from primary inputs or pseudo-inputs (D flip-flop Q outputs). Initially, the level of primary inputs and DFF flip-flops is set to zero. Gate levels for other gates are set to negative values, indicating uninitialized values. Then, the gate level (for gates with assigned positive values on all of its inputs) is equal to the maximum level of its inputs plus one. This step is repeated until every gate level is a positive number.

Your program should print the following:
a. For every level `n`, print the number of gates assigned to level n.
b. Print a listing of the final stored circuit using an intermediate file format. The file consists of lines, where each line represents a gate in the circuit. On the first line of the file, print the number of gates, the number of inputs, the number of outputs, and the number of flip-flops.

### Part 2: Simulator
Write a simulation program that reads the intermediate file generated in Part 1 and stores the gates into an array of records. In each record, dynamically allocate integer arrays to store the fanin and fanout for each gate. Create an extra gate structure and store its index in the variable 'dummy_gate'.
A. Add to every gate structure an int field called 'sched' and set it initially to -1. This field should be used to schedule the corresponding gate.
B. Create an array 'levels' of pointers to gate structures of size 'max_level'. 'max_level' is an integer holding the value of the maximum level in your design. Initially, levels[i] is set to dummy_gate for all i.
C. Use 3-valued logic {0, 1, X} to create two-input lookup tables for the following gates: AND, OR, XOR, and NOT. Gates with more than two inputs can be evaluated using more than one table lookup.
D. To schedule events due to a change in the state of gate i:

For each gate f in the fanout list of gate i, if the 'sched' field of gate f is -1, then insert f at the head of the list at the corresponding level. Otherwise, no action is needed (schedule_fanout(gaten) in Figure P1).

E. Implement the algorithm shown in Figure P1. Note that flip-flops do not need to be scheduled in this algorithm.

Record the CPU time the program requires to run both example circuits posted on the website using input scanning and table-lookup methods. Compare the two techniques. The output should look like the following:
<p align="center">
    <img src="./simulation_flow.png" />
</p>

<p align="center">
    <img src="./input_scanning.png" />
</p>

<p align="center">
    <img src="./table_lookup.png" />
</p>


The output should look like the following:
<p align="center">
    <img src="./input:output.png" />
</p>

For more information, please go into each subdirectory for more detail.

---
