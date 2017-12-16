#include <stdio.h>
#include <stdlib.h>
#include "cg.h"

int main()
{
    FeedDict feed[2] = {
        {.key = "x", .val = 1},
        {.key = "y", .val = 10},
    };

    Node *W = create_variable("W");
    Node *b = create_variable("b");
    Node *x = create_placeholder("x");
    Node *y = create_placeholder("y");

    Node *Wx = node_mul(x, W, "Wx");
    Node *Wx_plus_b = node_add(Wx, b, "Wx_plus_b");

    node_info(W);
    node_info(b);

    printf("Before optimize:\n");
    float ans = node_eval(Wx_plus_b, feed, 1);
    printf("ans: %f\n", ans);

    Node *mse = node_mse(Wx_plus_b, y, "mse_loss");
    float loss = node_eval(mse, feed, 2);
    printf("loss: %f\n", loss);

    int i = 0;
    while(i < 100) {
        i++;
        node_optimize(mse, 0.01, feed, 2);
        loss = node_eval(mse, feed, 2);
        /* printf("step: %d, loss: %f\n", i, loss); */
    }

    printf("\nAfter optimize:\n");
    ans = node_eval(Wx_plus_b, feed, 1);
    printf("ans: %f\n", ans);
    printf("loss: %f\n", loss);

    float wval = node_eval(W, NULL, 0);
    float bval = node_eval(b, NULL, 0);
    printf("W: %f, b: %f\n", wval, bval);

    free(W);
    free(b);
    free(x);
    free(Wx);
    free(Wx_plus_b);
    free(y);
    return 0;
}
