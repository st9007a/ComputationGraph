#ifndef GRAPH_H
#define GRAPH_H

#include "matrix.h"

enum node_type {
	DL_CONST,
	DL_VAR,
	DL_PLACEHOLDER
};

enum node_func_type {
	DL_FUNC_NONE,

	DL_SCALAR_ADD,
	DL_SCALAR_MUL,
	DL_SCALAR_SUB,
	DL_SCALAR_DIV,

	DL_COST_MSE,
};

typedef struct __DL_NODE Node;
typedef struct __DL_FEED_DICT FeedDict;

#define DL_NODE_NAME_LEN 128

struct __DL_NODE {
	enum node_type type;
	char name[DL_NODE_NAME_LEN];
	Matrix data;
	Matrix grad;
	struct {
		enum node_func_type type;
		Node *args[2];
	} expr;
	Node *ref;
};

struct __DL_FEED_DICT {
	char key[DL_NODE_NAME_LEN];
	float val;
};

void node_info(Node *n);

Node *node_variable(uint32_t *dim, uint32_t num_dims, char *name);
Node *node_constant(float *val, uint32_t *dim, uint32_t num_dims, char *name);
Node *node_placeholder(uint32_t *dim, uint32_t num_dims, char *name);

Node *scalar_add(Node *n1, Node *n2, char *name);
Node *scalar_mul(Node *n1, Node *n2, char *name);
Node *scalar_div(Node *n1, Node *n2, char *name);
Node *scalar_sub(Node *n1, Node *n2, char *name);

Node *cost_mse(Node *n1, Node *n2, char *name);

float node_eval(Node *target, FeedDict *feed, size_t len);
void node_optimize(Node *target, float lr, FeedDict *feed, size_t len);

#endif
