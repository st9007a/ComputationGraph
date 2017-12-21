#ifndef DL_H
#define DL_H

#include <stdint.h>
#include <stddef.h>

enum node_type {
    DL_CONST,
    DL_VAR,
    DL_PLACEHOLDER
};

enum node_func_type {
    DL_FUNC_NONE,

    DL_SCALAR_ADD,
    DL_SCALAR_SUB,
    DL_SCALAR_MUL,
    DL_SCALAR_DIV,

    DL_MATRIX_ADD,
    DL_MATRIX_SUB,
    DL_MATRIX_MUL,

    DL_ACT_RELU,
    DL_ACT_SIGMOID,
    DL_ACT_SOFTMAX,

    DL_COST_MSE,
};

typedef struct __DL_MATRIX Matrix;
typedef struct __DL_NODE Node;
typedef struct __DL_FEED_DICT FeedDict;

#define DL_NODE_NAME_LEN 128

struct __DL_MATRIX {
    float *val;
    uint32_t dim[4];
    uint32_t num_dims;
    uint32_t len;
};

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
    size_t len;
    char key[DL_NODE_NAME_LEN];
    float *val;
};

extern const struct __DL_API_GATEWAY {
    Matrix *(*eval)(Node *target, FeedDict *feed, size_t feed_size);
    void (*optimize)(Node *target, float lr, FeedDict *feed, size_t feed_size);
    struct {
        void (*info)(Node *n, int ignore_val);
    } debug;
    struct {
        Node *(*variable)(uint32_t *dim, uint32_t num_dims, char *name);
        Node *(*placeholder)(uint32_t *dim, uint32_t num_dims, char *name);
        Node *(*constant)(float *data, uint32_t *dim, uint32_t num_dims, char *name);
    } node;
    struct {
        Node *(*add)(Node *n1, Node *n2, char *name);
        Node *(*sub)(Node *n1, Node *n2, char *name);
        Node *(*mul)(Node *n1, Node *n2, char *name);
        Node *(*div)(Node *n1, Node *n2, char *name);
    } scalar;
    struct {
        Node *(*add)(Node *n1, Node *n2, char *name);
        Node *(*sub)(Node *n1, Node *n2, char *name);
        Node *(*mul)(Node *n1, Node *n2, char *name);
    } matrix;
    struct {
        Node *(*relu)(Node *preact, char *name);
        Node *(*sigmoid)(Node *preact, char *name);
        Node *(*softmax)(Node *preact, char *name);
    } activate;
    struct {
        Node *(*mse)(Node *logits, Node *target, char *name);
    } loss;
} dl;
#endif
