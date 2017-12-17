#include <stdlib.h>
#include "graph.h"

int main ()
{
    int x_dim = 10;
    int w_dim[2] = { 10, 2 };
    Node *x = node_placeholder(&x_dim, 1, "x");
    Node *w = node_variable(w_dim, 2, "w");

    Node *s = node_variable(NULL, 1, "s");

    Node *m = scalar_add(x, s, "ScalarAdd");

    node_info(x, 1);
    node_info(w, 1);
    node_info(s, 0);
    node_info(m, 0);

    free(x);
    free(w);
    free(s);
    free(m);
    return 0;
}
