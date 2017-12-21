#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "matrix.h"
#include "graph.h"

int main ()
{
    srand(time(NULL));
    int x_dim[2] = {1, 2};
    float x_val[2] = {1.0, 2.0};

    int y_dim[2] = {1, 3};
    float y_val[3] = {0, 1, 1};

    FeedDict feed[2] = {
        { .key = "x", .val = x_val, .len = 2, },
        { .key = "y", .val = y_val, .len = 3, },
    };

    Node *x = node_placeholder(x_dim, 2, "x");
    Node *y = node_placeholder(y_dim, 2, "y");

    int w_dim[2] = {2, 3};
    int b_dim[2] = {1, 3};
    Node *w = node_variable(w_dim, 2, "w");
    Node *b = node_variable(b_dim, 2, "b");

    Node *preact = node_matrix_add(
                       node_matrix_mul(x, w, "wx"),
                       b,
                       "wx_plus_b"
                   );

    Node *out = node_nn_sigmoid(preact, "out");

    printf("Before optimize: ");
    Matrix *res = node_eval(out, feed, 2);
    for (int i = 0; i < res->len; i++) {
        printf("%f ", res->val[i]);
    }
    printf("\n");

    Node *loss = node_cost_mse(out, y, "mse");

    for (int i = 0; i < 10000; i++) {
        node_optimize(loss, 0.01, feed, 2);
    }

    printf("After optimize: ");
    res = node_eval(out, feed, 2);
    for (int i = 0; i < res->len; i++) {
        printf("%f ", res->val[i]);
    }
    printf("\n");

    printf("True ans: ");
    for (int i = 0; i < 3; i++) {
        printf("%f ", y_val[i]);
    }
    printf("\n");

    return 0;
}
