#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "dl.h"

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

    Node *x = dl.tensor.placeholder(x_dim, 2, "x");
    Node *y = dl.tensor.placeholder(y_dim, 2, "y");

    int w1_dim[2] = {2, 4};
    int b1_dim[2] = {1, 4};

    int w2_dim[2] = {4, 3};
    int b2_dim[2] = {1, 3};

    Node *w1 = dl.tensor.variable(w1_dim, 2, "w1");
    Node *b1 = dl.tensor.variable(b1_dim, 2, "b1");
    Node *w2 = dl.tensor.variable(w2_dim, 2, "w2");
    Node *b2 = dl.tensor.variable(b2_dim, 2, "b2");

    Node *preact1 = dl.matrix.add(dl.matrix.mul(x, w1, ""), b1, "");
    Node *hidden = dl.activate.relu(preact1, "hidden");

    Node *preact2 = dl.matrix.add(dl.matrix.mul(hidden, w2, ""), b2, "");
    Node *out = dl.activate.softmax(preact2, "out");
    Node *loss = dl.loss.mse(out, y, "mse");

    printf("Before optimize:\n");
    dl.eval(out, feed, 2);
    dl.debug.info(out, 0);

    for (int i = 0; i < 10000; i++) {
        dl.optimize(loss, 0.005, feed, 2);
    }

    printf("After optimize:\n");
    dl.eval(out, feed, 2);
    dl.debug.info(out, 0);

    printf("True ans: ");
    for (int i = 0; i < 3; i++) {
        printf("%f ", y_val[i]);
    }
    printf("\n");

    return 0;
}
