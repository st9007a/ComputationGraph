#include "dl.h"
#include "matrix.h"
#include "graph.h"

const struct __DL_API_GATEWAY dl = {
    .eval = node_eval,
    .optimize = node_optimize,
    .debug = {
        .info = node_info,
    },
    .tensor = {
        .variable = node_variable,
        .constant = node_constant,
        .placeholder = node_placeholder,
    },
    .scalar = {
        .add = node_scalar_add,
        .sub = node_scalar_sub,
        .mul = node_scalar_mul,
        .div = node_scalar_div,
    },
    .matrix = {
        .add = node_matrix_add,
        .sub = node_matrix_sub,
        .mul = node_matrix_mul,
    },
    .activate = {
        .relu = node_act_relu,
        .sigmoid = node_act_sigmoid,
        .softmax = node_act_softmax,
    },
    .loss = {
        .mse = node_cost_mse,
    },
};
