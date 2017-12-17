#include <stddef.h>
#include "base.h"
#include "matrix.h"
#include "graph.h"

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
                // update gradient
            }
            continue;
        }

        for (int i = 0; i < 2; i++) {
            if (ptr->expr.args[i] != NULL) {
                // calc gradient

                queue[tail] = ptr->expr.args[i];
                tail = (tail + 1) % QUEUE_SIZE;
            }
        }
    }

}
