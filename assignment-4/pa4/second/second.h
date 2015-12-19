#ifndef _second_h
#define _second_h

/**
 * Enum for all instruction operations
 */	
typedef enum {AND, OR, NOT, MULTIPLEXER, DECODER} OpType; 

/*
 * Instruction struct. Holds all data for an instruction
 *
 * 	op: OpType enum of the operation this instruction does
 * 	vars: array that holds variables
 * 	names: array that holds the variable letters this instruction outputs
 * 	inputs: array for inputs to multiplexer/decoder
 * 	numInputs: count of inputs
 * 	numOutputs: count of outputs
 *
 */
typedef struct instruction {
	OpType op;
	int *vars;
	char *names;
	int *inputs;
	int numInputs;
	int numOutputs;
} instruction;

/*
 * Node struct. Acts as a variable, has a variable letter and int value
 *
 * 	letter: char of the letter name for this variable
 * 	value: int value of the variable
 * 	next: next node in the linked list, if there are multiple nodes in this hash key
 *
 */ 
typedef struct node {
	char letter;
	int value;
	struct node *next;
} node;

/**
 * Hashtable struct. Holds array of nodes(variables)
 *	
 *	size: how many elements the table array can hold
 *	table: array of pointers to node structs
 *
 */
typedef struct hashtable {
	int size;
	struct node **table;
} hashtable;

/**
 * Function: evalInstruction
 * -------------------------
 * Evaluates an instruction, recursively calling itself on other instructions if inputs are unresolved variables
 * 
 * 	i: index of the instruction for the instruction array
 *
 */
void evalInstruction(int i);

/**
 * Function int2gray
 * -----------------
 *  Takes a base 10 integer and puts it into gray sequencing
 *
 *	n: int to change to gray sequencing
 *
 *  returns: int in gray sequencing
 */
int int2gray(int n);

/**
 * Function: runInputs
 * -------------------
 *  Runs all inputs through instruction set
 *
 */
void runInputs();

/**
 * Function: readInputs
 * --------------------
 *  Reads inputs from file
 *
 *  	fp: FILE pointer with inputs
 *
 */
void readInputs(FILE *fp);

/**
 * Function: readInstructions
 * --------------------------
 *  Reads instructions from file and creates instruction structs
 *
 *  	fp: FILE pointer with instructions
 *
 */
void readInstructions(FILE *fp);

/**
 * Function: cleanup
 * -----------------
 *  Called after all inputs are completed, frees used memory
 *
 */
void cleanup();

/**
 * Function: create_instruction
 * ----------------------------
 *  Allocates space for and creates an instruction struct
 *
 *  returns: created instruction struct
 *
 */
struct instruction *create_instruction();

/**
 * Function: destroy_table
 * -----------------------
 *  Deallocates everything in the instruction table
 *
 */
void destroy_table();

/**
 * Function: destroy_node
 * ----------------------
 *  Deallocates a node and all its children
 *
 *  	ptr: pointer to the node needing destroying
 *
 */
void destroy_node(struct node *ptr);

/**
 * Function: create_table
 * ----------------------
 *  Creates an empty hashtable
 *  	
 *  	size: how large the table array should be
 *
 */
void create_table(int size);

/**
 * Function: setVar
 * ----------------
 *  Creates a new node(variable) struct and inserts it into the hash table, or updates the value if it already exists
 *
 *  	letter: variable name
 *	value: int value of variable
 *
 */
void setVar(char letter, int value);

/**
 * Function: getVar
 * ----------------
 *  Gets the value of the variable in the hash table
 *
 *  	name: variable letter
 *
 *  returns: value of variable, or -1 if it doesn't exist
 *
 */
int getVar(char name);

#endif
