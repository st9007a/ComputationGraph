#include <assert.h>
#include <math.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "base.h"
#include "matrix.h"
#include "graph.h"

#define NODE_INIT(n, dim, num_dims, type)      \
    do {                                       \
        n = malloc(sizeof(Node));              \
        if (n == NULL) {                       \
            FATAL(MEMORY_EXHAUSTED_ERROR"\n"); \
        }                                      \
        strcpy(n->name, name);                 \
        MATRIX_INIT(n->data, dim, num_dims);   \
        MATRIX_INIT(n->grad, dim, num_dims);   \
        n->type = type;                        \
        n->expr.type = DL_FUNC_NONE;           \
        n->expr.args[0] = NULL;                \
        n->expr.args[1] = NULL;                \
        n->ref = NULL;                         \
    } while(0);

static inline float box_muller_sampling()
{
    int mean = 0;
    int std = 1;

    return sqrt(-2 * log(rand() / (float)RAND_MAX)) * cos(2 * M_PI * rand() / (float)RAND_MAX) * std + mean;
}

void node_info(Node *n)
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

    printf("Dimension: ");
    for (int i = 0; i < n->data.num_dims; i++) {
        printf("%f ", n->data.dim[i]);
    }
    printf("\n");

    printf("Value: ");
    for (int i = 0; i < n->data.len; i++) {
        printf("%f ", n->data.val[i]);
    }
    printf("\n");

    printf("Expression Type: ");
    if (n->expr.type == DL_FUNC_NONE) {
        printf("None\n");
    }
    else {
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
    }

    if (n->ref != NULL) {
        printf("Ref: %s\n", n->ref->name);
    }
    printf("---------------------\n");
}

Node *node_variable(uint32_t *dim, uint32_t num_dims, char *name)
{
    Node *n;
    NODE_INIT(n, dim, num_dims, DL_VAR);

    for (int i = 0; i < n->data.len; i++) {
        n->data.val[i] = box_muller_sampling();
    }

    return n;
}

Node *node_constant(float *data, uint32_t *dim, uint32_t num_dims, char *name)
{
    Node *n;
    NODE_INIT(n, dim, num_dimsi, DL_CONST);

    memcpy(n->data.val, data, sizeof(float) * n->data.len);

    return n;
}

Node *node_placeholder(uint32_t *dim, uint32_t num_dims, char *name)
{
    Node *n = malloc(sizeof(Node));
    NODE_INIT(n, dim, num_dims, DL_PLACEHOLDER);

    return n;
}

Node *node_add(Node *n1, Node *n2, char *name)
{
    Node *n = malloc(sizeof(Node));

    strcpy(n->name, name);

    if (n1->type == CONST && n2->type == CONST) {
        n->type = CONST;
        n->val = n1->val + n2->val;
    } else {
        n->type = PLACEHOLDER;
    }

    n->expr.type = ADD;
    n->expr.args[0] = n1;
    n->expr.args[1] = n2;
    n->ref = NULL;

    n1->ref = n;
    n2->ref = n;

    return n;
}

Node *node_sub(Node *n1, Node *n2, char *name)
{
    Node *n = malloc(sizeof(Node));

    strcpy(n->name, name);
    n->ref = NULL;
    n1->ref = n;
    n2->ref = n;

    if (n1->type == CONST && n2->type == CONST) {
        n->type = CONST;
        n->val = n1->val - n2->val;
        n->expr.type = NONE;
        return n;
    }

    n->type = PLACEHOLDER;
    n->expr.type = SUB;
    n->expr.args[0] = n1;
    n->expr.args[1] = n2;

    return n;
}

Node *node_mul(Node *n1, Node *n2, char *name)
{
    Node *n = malloc(sizeof(Node));

    strcpy(n->name, name);
    n->ref = NULL;
    n1->ref = n;
    n2->ref = n;

    if (n1->type == CONST && n2->type == CONST) {
        n->type = CONST;
        n->val = n1->val * n2->val;
        n->expr.type = NONE;
        return n;
    }

    n->type = PLACEHOLDER;
    n->expr.type = MUL;
    n->expr.args[0] = n1;
    n->expr.args[1] = n2;

    return n;
}

Node *node_div(Node *n1, Node *n2, char *name)
{
    Node *n = malloc(sizeof(Node));

    strcpy(n->name, name);
    n->ref = NULL;
    n1->ref = n;
    n2->ref = n;

    if (n1->type == CONST && n2->type == CONST) {
        n->type = CONST;
        n->val = n1->val / n2->val;
        n->expr.type = NONE;
        return n;
    }

    n->type = PLACEHOLDER;
    n->expr.type = DIV;
    n->expr.args[0] = n1;
    n->expr.args[1] = n2;

    return n;
}

Node *node_mse(Node *n1, Node *n2, char *name)
{
    Node *n = malloc(sizeof(Node));

    strcpy(n->name, name);
    n->ref = NULL;
    n1->ref = n;
    n2->ref = n;

    if (n1->type == CONST && n2->type == CONST) {
        n->type = CONST;
        n->val = sqrt(pow(n1->val - n2->val, 2));
        n->expr.type = NONE;
        return n;
    }

    n->type = PLACEHOLDER;
    n->expr.type = MSE;
    n->expr.args[0] = n1;
    n->expr.args[1] = n2;

    return n;
}

float node_eval(Node *target, FeedDict *feed, size_t len)
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
