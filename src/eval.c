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

static inline void eval_scalar_add(Matrix *res, Matrix *m1, Matrix *m2, int diff)
{
    matrix_scalar_add(res, m1, m2->val[0], diff);
}

static inline void eval_scalar_sub(Matrix *res, Matrix *m1, Matrix *m2, int diff)
{
    matrix_scalar_sub(res, m1, m2->val[0], diff);
}


static inline void eval_scalar_mul(Matrix *res, Matrix *m1, Matrix *m2, int diff)
{
    matrix_scalar_mul(res, m1, m2->val[0], diff);
}


static inline void eval_scalar_div(Matrix *res, Matrix *m1, Matrix *m2, int diff)
{
    matrix_scalar_div(res, m1, m2->val[0], diff);
}

static inline void eval_cost_mse(Matrix *res, Matrix *m1, Matrix *m2, int diff)
{
    matrix_cost_mse(res, m1, m2, diff);
}

const struct __DL_EVAL_FUNC_HOOK eval_funcs[6] = {
    { DL_FUNC_NONE, eval_func_none },
    { DL_SCALAR_ADD, eval_scalar_add },
    { DL_SCALAR_SUB, eval_scalar_sub },
    { DL_SCALAR_MUL, eval_scalar_mul },
    { DL_SCALAR_DIV, eval_scalar_div },
    { DL_COST_MSE, eval_cost_mse },
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
