#ifndef CG_H
#define CG_H

enum node_type {
	CONST,
	VAR,
	PLACEHOLDER
};

enum node_func_type {
	NONE,
	ADD,
	MUL,
	SUB,
	DIV,
};

typedef struct __CG_NODE Node;

struct __CG_NODE {
	enum node_type type;
	char name[128];
	float val;
	float grad;
	struct {
		enum node_func_type type;
		Node *args[2];
	} expr;
	Node *ref;
};

void node_info(Node *n);

Node *create_variable(char *name);
Node *create_constant(char *name, float val);
Node *create_placeholder(char *name);

Node *node_add(Node *n1, Node *n2, char *name);
Node *node_mul(Node *n1, Node *n2, char *name);
Node *node_div(Node *n1, Node *n2, char *name);
Node *node_sub(Node *n1, Node *n2, char *name);

void optimize(Node *root);

#endif
