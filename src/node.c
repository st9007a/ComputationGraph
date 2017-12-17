#include <assert.h>
#include <math.h>
#include <memory.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "base.h"
#include "matrix.h"
#include "graph.h"

#define NODE_INIT(n, dim, num_dims, TYPE)      \
    do {                                       \
        n = malloc(sizeof(Node));              \
        if (n == NULL) {                       \
            FATAL(MEMORY_EXHAUSTED_ERROR"\n"); \
        }                                      \
        strcpy(n->name, name);                 \
        MATRIX_INIT(n->data, dim, num_dims);   \
        MATRIX_INIT(n->grad, dim, num_dims);   \
        n->type = TYPE;                        \
        n->expr.type = DL_FUNC_NONE;           \
        n->expr.args[0] = NULL;                \
        n->expr.args[1] = NULL;                \
        n->ref = NULL;                         \
    } while(0);

void node_info(Node *n, int ignore_val)
{
    printf("---------------------\n");
    printf("Name: %s\n", n->name);
    printf("---------------------\n");
    printf("Type: ");
    switch (n->type) {
    case DL_CONST:
        printf("Constant\n");
        break;
    case DL_VAR:
        printf("Variable\n");
        break;
    case DL_PLACEHOLDER:
        printf("Placeholder\n");
        break;
    default:
        printf("Unknown type\n");
    }

    printf("Dimension: %d-D\n", n->data.num_dims);
    printf("Dimension Lenght: ");
    for (int i = 0; i < n->data.num_dims; i++) {
        printf("%u ", n->data.dim[i]);
    }
    printf("\n");

    if (ignore_val)
        goto print_expr;

    printf("Value: ");
    for (int i = 0; i < n->data.len; i++) {
        printf("%f ", n->data.val[i]);
    }
    printf("\n");

print_expr:

    printf("Expression Type: ");
    if (n->expr.type == DL_FUNC_NONE) {
        printf("None\n");
        goto print_ref;
    }

    switch (n->expr.type) {
    case DL_SCALAR_ADD:
        printf("Add\n");
        break;
    case DL_SCALAR_SUB:
        printf("Sub\n");
        break;
    case DL_SCALAR_MUL:
        printf("Mul\n");
        break;
    case DL_SCALAR_DIV:
        printf("Div\n");
        break;
    case DL_COST_MSE:
        printf("Mean Square Error\n");
    default:
        printf("Unknown\n");
    }

    printf("Arg 1: %s\n", n->expr.args[0]->name);
    printf("Arg 2: %s\n", n->expr.args[1]->name);

print_ref:

    if (n->ref != NULL) {
        printf("Ref: %s\n", n->ref->name);
    }
    printf("---------------------\n");
}

Node *node_variable(uint32_t *dim, uint32_t num_dims, char *name)
{
    Node *n;
    NODE_INIT(n, dim, num_dims, DL_VAR);

    matrix_init_random_norm(&n->data);

    return n;
}

Node *node_constant(float *data, uint32_t *dim, uint32_t num_dims, char *name)
{
    Node *n;
    NODE_INIT(n, dim, num_dims, DL_CONST);

    matrix_init_constant(&n->data, data, n->data.len);

    return n;
}

Node *node_placeholder(uint32_t *dim, uint32_t num_dims, char *name)
{
    Node *n;
    NODE_INIT(n, dim, num_dims, DL_PLACEHOLDER);

    return n;
}

Node *node_scalar_add(Node *n1, Node *n2, char *name)
{
    if (n2->data.num_dims != 0) {
        FATAL(UNEXPECTED_SHAPE_ERROR": scalar calculation require a 0-D Node\n");
    }

    Node *n = node_placeholder(n1->data.dim, n1->data.num_dims, name);

    if (n1->type == DL_CONST && n2->type == DL_CONST) {
        n->type = DL_CONST;
        for (int i = 0; i < n1->data.len; i++) {
            n->data.val[i] = n1->data.val[i] + n2->data.val[0];
        }
    }

    n->expr.type = DL_SCALAR_ADD;
    n->expr.args[0] = n1;
    n->expr.args[1] = n2;
    n->ref = NULL;

    n1->ref = n;
    n2->ref = n;

    return n;
}

Node *node_scalar_sub(Node *n1, Node *n2, char *name)
{
    if (n2->data.num_dims != 0) {
        FATAL(UNEXPECTED_SHAPE_ERROR": scalar calculation require a 0-D Node\n");
    }

    Node *n = node_placeholder(n1->data.dim, n1->data.num_dims, name);

    if (n1->type == DL_CONST && n2->type == DL_CONST) {
        n->type = DL_CONST;
        for (int i = 0; i < n1->data.len; i++) {
            n->data.val[i] = n1->data.val[i] - n2->data.val[0];
        }
    }

    n->expr.type = DL_SCALAR_SUB;
    n->expr.args[0] = n1;
    n->expr.args[1] = n2;
    n->ref = NULL;

    n1->ref = n;
    n2->ref = n;

    return n;
}

Node *node_scalar_mul(Node *n1, Node *n2, char *name)
{
    if (n2->data.num_dims != 0) {
        FATAL(UNEXPECTED_SHAPE_ERROR": scalar calculation require a 0-D Node\n");
    }

    Node *n = node_placeholder(n1->data.dim, n1->data.num_dims, name);

    if (n1->type == DL_CONST && n2->type == DL_CONST) {
        n->type = DL_CONST;
        for (int i = 0; i < n1->data.len; i++) {
            n->data.val[i] = n1->data.val[i] * n2->data.val[0];
        }
    }

    n->expr.type = DL_SCALAR_MUL;
    n->expr.args[0] = n1;
    n->expr.args[1] = n2;
    n->ref = NULL;

    n1->ref = n;
    n2->ref = n;

    return n;
}

Node *node_scalar_div(Node *n1, Node *n2, char *name)
{
    if (n2->data.num_dims != 0) {
        FATAL(UNEXPECTED_SHAPE_ERROR": scalar calculation require a 0-D Node\n");
    }

    Node *n = node_placeholder(n1->data.dim, n1->data.num_dims, name);

    if (n1->type == DL_CONST && n2->type == DL_CONST) {
        n->type = DL_CONST;
        for (int i = 0; i < n1->data.len; i++) {
            n->data.val[i] = n1->data.val[i] / n2->data.val[0];
        }
    }

    n->expr.type = DL_SCALAR_DIV;
    n->expr.args[0] = n1;
    n->expr.args[1] = n2;
    n->ref = NULL;

    n1->ref = n;
    n2->ref = n;

    return n;
}

/*
float *node_eval(Node *target, FeedDict *feed, size_t len)
{
    if (target->type == CONST || target->type == VAR) {
        return target->val;
    }

    switch (target->expr.type) {
        case ADD:
            return node_eval(target->expr.args[0], feed, len) + node_eval(target->expr.args[1], feed, len);
        case SUB:
            return node_eval(target->expr.args[0], feed, len) - node_eval(target->expr.args[1], feed, len);
        case MUL:
            return node_eval(target->expr.args[0], feed, len) * node_eval(target->expr.args[1], feed, len);
        case DIV:
            return node_eval(target->expr.args[0], feed, len) / node_eval(target->expr.args[1], feed, len);
        case MSE:
            return sqrt(pow(node_eval(target->expr.args[0], feed, len) - node_eval(target->expr.args[1], feed, len), 2));
        case NONE:
            for (int i = 0; i < len; i++) {
                if (!strcmp(target->name, feed[i].key)) {
                    target->val = feed[i].val;
                    return feed[i].val;
                }
            }
            printf("Feed data not found\n");
            exit(1);

        default:
            printf("Unknown expression type\n");
            exit(1);
    }
}

static float node_calc(Node *target)
{
    switch (target->expr.type) {
        case NONE:
            return target->val;
        case ADD:
            return target->expr.args[0]->val + target->expr.args[1]->val;
        case SUB:
            return target->expr.args[0]->val - target->expr.args[1]->val;
        case MUL:
            return target->expr.args[0]->val * target->expr.args[1]->val;
        case DIV:
            return target->expr.args[0]->val / target->expr.args[1]->val;

        case MSE:
            return sqrt(pow(target->expr.args[0]->val - target->expr.args[1]->val, 2));
        default:
            printf("Unknown expression type\n");
            exit(1);
    }
}

static float differential(Node *y, Node *x)
{

    assert(x == y->expr.args[0] || x == y->expr.args[1]);

    const float esp = 0.0001;

    float up, low;
    float store_x = x->val;

    x->val = store_x + esp;
    up = node_calc(y);

    x->val = store_x - esp;
    low = node_calc(y);

    x->val = store_x;

    return (up - low) / (2 * esp);
}

void node_optimize(Node *target, float lr, FeedDict *feed, size_t len)
{
    Node *queue[32];
    Node *ptr;
    int head = 0;
    int tail = 0;

    node_eval(target, feed, len);

    target->grad = 1;
    queue[tail++] = target;

    while (head != tail) {
        ptr = queue[head];
        head = (head + 1) % 32;
        if (ptr->expr.type == NONE) {
            if (ptr->type == VAR) {
                ptr->val = ptr->val - ptr->grad * lr;
            }
            continue;
        }

        for (int i = 0; i < 2; i++) {
            if (ptr->expr.args[i] != NULL) {
                ptr->expr.args[i]->grad = ptr->grad * differential(ptr, ptr->expr.args[i]);
                queue[tail] = ptr->expr.args[i];
                tail = (tail + 1) % 32;
            }
        }
    }

}
*/
