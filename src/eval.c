#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "base.h"
#include "matrix.h"
#include "graph.h"

static inline void eval_func_none(Matrix *res, Matrix *m1, Matrix *m2, int diff)
{

}

const struct __DL_EVAL_FUNC_HOOK eval_funcs[12] = {
    { DL_FUNC_NONE, eval_func_none },
    { DL_SCALAR_ADD, matrix_scalar_add },
    { DL_SCALAR_SUB, matrix_scalar_sub },
    { DL_SCALAR_MUL, matrix_scalar_mul },
    { DL_SCALAR_DIV, matrix_scalar_div },
    { DL_MATRIX_ADD, matrix_add },
    { DL_MATRIX_SUB, matrix_sub },
    { DL_MATRIX_MUL, matrix_mul },
    { DL_NN_RELU, matrix_nn_relu },
    { DL_NN_SIGMOID, matrix_nn_sigmoid },
    { DL_NN_SOFTMAX, matrix_nn_softmax },
    { DL_COST_MSE, matrix_cost_mse },
};

Matrix *node_eval(Node *target, FeedDict *feed, size_t feed_size)
{
    if (target == NULL) {
        return NULL;
    } else if (target->type == DL_VAR || target->type == DL_CONST) {
        return &target->data;
    } else if (target->expr.type == DL_FUNC_NONE) {
        for (int i = 0; i < feed_size; i++) {
            if (!strcmp(feed[i].key, target->name)) {
                memcpy(target->data.val, feed[i].val, feed[i].len * sizeof(float));
                return &target->data;
            }
        }
        FATAL(FEED_DATA_ERROR": Feed data not found\n");
    }

    eval_funcs[target->expr.type].op_func(
        &target->data,
        node_eval(target->expr.args[0], feed, feed_size),
        node_eval(target->expr.args[1], feed, feed_size),
        0
    );

    return &target->data;
}
