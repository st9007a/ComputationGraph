#ifndef GRAPH_H
#define GRAPH_H

#include <stdint.h>
#include "dl.h"
#include "matrix.h"

void node_info(Node *n, int ignore_val);

Node *node_variable(uint32_t *dim, uint32_t num_dims, char *name);
Node *node_constant(float *val, uint32_t *dim, uint32_t num_dims, char *name);
Node *node_placeholder(uint32_t *dim, uint32_t num_dims, char *name);

Node *node_scalar_add(Node *n1, Node *n2, char *name);
Node *node_scalar_mul(Node *n1, Node *n2, char *name);
Node *node_scalar_div(Node *n1, Node *n2, char *name);
Node *node_scalar_sub(Node *n1, Node *n2, char *name);

Node *node_matrix_add(Node *n1, Node *n2, char *name);
Node *node_matrix_sub(Node *n1, Node *n2, char *name);
Node *node_matrix_mul(Node *n1, Node *n2, char *name);

Node *node_shape_reshape(Node *n, uint32_t *dim, uint32_t num_dims, char *name);

Node *node_act_relu(Node *preact, char *name);
Node *node_act_sigmoid(Node *preact, char *name);
Node *node_act_softmax(Node *preact, char *name);

Node *node_cost_mse(Node *logits, Node *target, char *name);

extern const struct __DL_EVAL_FUNC_HOOK {
    int idx;
    void (*op_func)(Matrix *res, Matrix *m1, Matrix *m2, int diff);
} eval_funcs[13];

Matrix *node_eval(Node *target, FeedDict *feed, size_t feed_size);
void node_optimize(Node *target, float lr, FeedDict *feed, size_t feed_size);

#endif
