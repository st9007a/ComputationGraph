#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"
#include "graph.h"

int main ()
{
    int x_dim[2] = {2, 2};
    float x_val[4] = {1.0, 2.0, 3.0, 4.0};
    FeedDict feed = {
        .key = "x",
        .val = x_val,
        .len = 4,
    };

    Node *x = node_placeholder(x_dim, 2, "x");
    Node *w = node_variable(NULL, 0, "w");
    Node *b = node_variable(NULL, 0, "b");

    node_info(w, 0);
    node_info(b, 0);

    Node *wx = node_scalar_mul(x, w, "wx");
    Node *wx_plus_b = node_scalar_add(wx, b, "wx_plus_b");

    Matrix *res = node_eval(wx_plus_b, &feed, 1);
    for (int i = 0; i < res->len; i++) {
        printf("%f ", res->val[i]);
    }
    printf("\n");

    free(x);
    free(w);
    free(b);
    free(wx);
    free(wx_plus_b);
    return 0;
}
