#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "matrix.h"
#include "graph.h"

int main ()
{
    srand(time(NULL));
    int x_dim[2] = {2, 2};
    float x_val[4] = {1.0, 2.0, 3.0, 4.0};

    int y_dim[2] = {2, 2};
    float y_val[4] = {2.9, 5.05, 7.2, 8.88};

    FeedDict feed[2] = {
        { .key = "x", .val = x_val, .len = 4, },
        { .key = "y", .val = y_val, .len = 4, },
    };

    Node *x = node_placeholder(x_dim, 2, "x");
    Node *y = node_placeholder(y_dim, 2, "y");
    Node *w = node_variable(NULL, 0, "w");
    Node *b = node_variable(NULL, 0, "b");


    Node *wx = node_scalar_mul(x, w, "wx");
    Node *wx_plus_b = node_scalar_add(wx, b, "wx_plus_b");

    Node *loss = node_cost_mse(wx_plus_b, y, "loss");

    Matrix *res = node_eval(wx_plus_b, feed, 1);

    node_info(w, 0);
    node_info(b, 0);
    printf("Before optimize: ");
    for (int i = 0; i < res->len; i++) {
        printf("%f ", res->val[i]);
    }
    printf("\n");

    for (int i = 0; i < 1000; i++) {
        node_optimize(loss, 0.001, feed, 2);
    }

    res = node_eval(wx_plus_b, feed, 1);
    node_info(w, 0);
    node_info(b, 0);
    printf("After optimize: ");
    for (int i = 0; i < res->len; i++) {
        printf("%f ", res->val[i]);
    }
    printf("\n");

    free(x);
    free(w);
    free(b);
    free(wx);
    free(wx_plus_b);
    free(y);
    free(loss);
    return 0;
}
