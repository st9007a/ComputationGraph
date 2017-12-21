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
    float y_val[3] = {0, 1, 0};

    FeedDict feed[2] = {
        { .key = "x", .val = x_val, .len = 2, },
        { .key = "y", .val = y_val, .len = 3, },
    };

    Node *x = node_placeholder(x_dim, 2, "x");
    Node *y = node_placeholder(y_dim, 2, "y");

    int w1_dim[2] = {2, 4};
    int b1_dim[2] = {1, 4};

    int w2_dim[2] = {4, 3};
    int b2_dim[2] = {1, 3};

    Node *w1 = node_variable(w1_dim, 2, "w1");
    Node *b1 = node_variable(b1_dim, 2, "b1");
    Node *w2 = node_variable(w2_dim, 2, "w2");
    Node *b2 = node_variable(b2_dim, 2, "b2");

    Node *preact1 = node_matrix_add(node_matrix_mul(x, w1, ""), b1, "");
    Node *hidden = node_nn_relu(preact1, "hidden");

    Node *preact2 = node_matrix_add(node_matrix_mul(hidden, w2, ""), b2, "");
    Node *out = node_nn_softmax(preact2, "out");

    printf("Before optimize: ");
    Matrix *res = node_eval(out, feed, 2);
    for (int i = 0; i < res->len; i++) {
        printf("%f ", res->val[i]);
    }
    printf("\n");

    Node *loss = node_cost_mse(out, y, "mse");

    for (int i = 0; i < 10000; i++) {
        node_optimize(loss, 0.005, feed, 2);
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
