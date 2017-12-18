#include <memory.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graph.h"

void test_node_create_op(float *data, uint32_t *dim, uint32_t num_dims)
{
    Node *var = node_variable(dim, num_dims, "var");
    if (var->type != DL_VAR ||
            var->data.num_dims != num_dims ||
            strcmp(var->name, "var") ||
            memcmp(dim, var->data.dim, sizeof(int) * num_dims)) {

        printf("Test node_variable(): Fail\n");
        exit(1);
    }

    printf("Test node_variable(): Success\n");

    Node *hold = node_placeholder(dim, num_dims, "holder");
    if (hold->type != DL_PLACEHOLDER ||
            hold->data.num_dims != num_dims ||
            strcmp(hold->name, "holder") ||
            memcmp(dim, hold->data.dim, sizeof(int) * num_dims)) {

        printf("Test node_placeholder(): Fail\n");
        exit(1);
    }

    printf("Test node_placeholder(): Success\n");

    Node *con = node_constant(data, dim, num_dims, "const");
    if (con->type != DL_CONST ||
            con->data.num_dims != num_dims ||
            strcmp(con->name, "const") ||
            memcmp(dim, con->data.dim, sizeof(int) * num_dims) ||
            memcmp(data, con->data.val, sizeof(float) * con->data.len)) {

        printf("Test node_constant(): Fail\n");
        exit(1);
    }

    printf("Test node_constant(): Success\n");

    free(var);
    free(hold);
    free(con);
}

void test_node_scalar_op(uint32_t *dim, uint32_t num_dims)
{
    Node *n1 = node_placeholder(dim, num_dims, "node1");
    Node *n2 = node_variable(NULL, 0, "scalar");

    Node *add = node_scalar_add(n1, n2, "scalar_add");

    if (add->type != DL_PLACEHOLDER ||
            add->data.num_dims != n1->data.num_dims ||
            memcmp(add->data.dim, n1->data.dim, sizeof(int) * add->data.num_dims)) {

        printf("Test node_scalar_add(): Fail\n");
        exit(1);
    }

    printf("Test node_scalar_add(): Success\n");

    Node *sub = node_scalar_sub(n1, n2, "scalar_sub");

    if (sub->type != DL_PLACEHOLDER ||
            sub->data.num_dims != n1->data.num_dims ||
            memcmp(sub->data.dim, n1->data.dim, sizeof(int) * sub->data.num_dims)) {

        printf("Test node_scalar_sub(): Fail\n");
        exit(1);
    }

    printf("Test node_scalar_sub(): Success\n");

    Node *mul = node_scalar_mul(n1, n2, "scalar_mul");

    if (mul->type != DL_PLACEHOLDER ||
            mul->data.num_dims != n1->data.num_dims ||
            memcmp(mul->data.dim, n1->data.dim, sizeof(int) * mul->data.num_dims)) {

        printf("Test node_scalar_mul(): Fail\n");
        exit(1);
    }

    printf("Test node_scalar_mul(): Success\n");

    Node *div = node_scalar_div(n1, n2, "scalar_div");

    if (div->type != DL_PLACEHOLDER ||
            div->data.num_dims != n1->data.num_dims ||
            memcmp(div->data.dim, n1->data.dim, sizeof(int) * div->data.num_dims)) {

        printf("Test node_scalar_div(): Fail\n");
        exit(1);
    }

    printf("Test node_scalar_div(): Success\n");

    free(n1);
    free(n2);
    free(add);
    free(sub);
    free(mul);
    free(div);
}

int main()
{
    float test_data[4] = { 2.0, 2.0, 3.0, 4.0 };
    uint32_t test_dim[2] = { 2, 2 };

    test_node_create_op(test_data, test_dim, 2);
    test_node_scalar_op(test_dim, 2);

    return 0;
}
