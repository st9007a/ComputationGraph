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

    float ans = node_eval(Wx_plus_b, feed, 1);
    printf("ans: %f\n", ans);

    Node *mse = node_mse(Wx_plus_b, y, "mse_loss");
    float loss = node_eval(mse, feed, 2);
    printf("loss: %f\n", loss);

    free(W);
    free(b);
    free(x);
    free(Wx);
    free(Wx_plus_b);
    free(y);
    return 0;
}
