#include <stddef.h>
#include "base.h"
#include "matrix.h"
#include "graph.h"

#define EPSILSON 0.0001
#define eval(node, diff)                 \
    eval_funcs[node->expr.type].op_func( \
        &node->data,                     \
        &node->expr.args[0]->data,       \
        &node->expr.args[1]->data,       \
        (diff)                           \
    );

static void node_partial_diff(Node *func, Node *x)
{
    for (int i = 0; i < x->data.len; i++) {
        float hold = x->data.val[i];
        x->grad.val[i] = 0;
        x->data.val[i] = hold + EPSILSON;

        eval(func, 0);
        x->data.val[i] = hold - EPSILSON;

        eval(func, 1);
        for (int j = 0; j < func->data.len; j++) {
            x->grad.val[i] += (func->data.val[j] / (2 * EPSILSON)) * func->grad.val[j];
        }
        x->data.val[i] = hold;
    }
}

void node_optimize(Node *target, float lr, FeedDict *feed, size_t feed_size)
{
#define QUEUE_SIZE 32
    Node *queue[QUEUE_SIZE];
    Node *ptr;
    int head = 0, tail = 0;

    node_eval(target, feed, feed_size);
    matrix_init_ones(&target->grad);

    queue[tail++] = target;

    while (head != tail) {
        ptr = queue[head];
        head = (head + 1) % QUEUE_SIZE;

        if (ptr->expr.type == DL_FUNC_NONE) {
            if (ptr->type == DL_VAR) {
                for (int i = 0; i < ptr->data.len; i++) {
                    ptr->data.val[i] -= ptr->grad.val[i] * lr;
                }
            }
            continue;
        }

        for (int i = 0; i < 2; i++) {
            if (ptr->expr.args[i] != NULL) {
                node_partial_diff(ptr, ptr->expr.args[i]);

                queue[tail] = ptr->expr.args[i];
                tail = (tail + 1) % QUEUE_SIZE;
            }
        }
    }
}
