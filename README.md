# Deep Learning Framework

## Introduction

A deep learning framework with c language.
I try to build a deep learning framework with few features to reduce the usage of cpu.
And let it has a user-friendly api.

## Feature

- Shared library
- Lite
- Tensorflow-like programming pattern 

## Future work

- Add a csv reader
- Add convolution and pooling
- Design a more robust algorithm for backpropagation
- Improve the precision of the partial differential
- Improve the speed of matrix calculation

## Demo

A simple neural network demo

1. `make`
2. `make demo`

## Quick start

1. Type command `make` to build shared library
2. Create a c script `test.c` and write down the below:

```c
#include <stdlib.h>
#include "dl.h"

int main()
{
    int dim[2] = {2, 3};
    float constant[6] = {1, 2, 3, 4, 5, 6};
    Node *n = dl.node.constant(constant, dim, 2, "Hello World");

    dl.debug.info(n, 0);

    free(n);
    return 0;
}
```

3. Type command `gcc -o test test.c lib/libdl.so -Isrc -Wl,-rpath=lib -lm` to compile
4. Type command `./test` and see result:

```
---------------------
Name: Hello World
---------------------
Type: Constant
Dimension: 2-D
Dimension Lenght: 2 3
Value: 1.000000 2.000000 3.000000 4.000000 5.000000 6.000000
Expression Type: None
---------------------
```
