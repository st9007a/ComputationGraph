#include <stdio.h>
#include <stdlib.h>
#include "cg.h"

int main()
{
    FeedDict feed = {.key = "x", .val = 1};

    Node *W = create_variable("W");
    Node *b = create_variable("b");
    Node *x = create_placeholder("x");

    Node *Wx = node_mul(x, W, "Wx");
    Node *Wx_plus_b = node_add(Wx, b, "Wx_plus_b");

    node_info(W);
    node_info(b);
    node_info(x);
    node_info(Wx);
    node_info(Wx_plus_b);

    float ans = node_eval(Wx_plus_b, &feed, 1);
    printf("ans: %f\n", ans);


    free(W);
    free(b);
    free(x);
    free(Wx);
    free(Wx_plus_b);
    return 0;
}
