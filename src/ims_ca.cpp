/*********************************
 * IMS - cellular automata       *
 * 2018/2019                     *
 * Tomas Willaschek  (xwilla00)  *
 * Adam Lanicek      (xlanic04)  *
 *********************************/

#include "ims_ca.h"
#include "model.h"

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <cerrno>

using std::cerr;
using std::endl;

int main(int argc, char **argv){
    int c, matrix_size = 0, alpha = 1, beta = 1, fermions = 0;

    while ((c = getopt(argc, argv, "a:b:m:f:")) != -1)
        switch (c) {
            case 'a':
                alpha = atoi(optarg);
                break;
            case 'b':
                beta = atoi(optarg);
                break;
            case 'm':
                matrix_size = atoi(optarg);
                break;
            case 'f':
                fermions = atoi(optarg);
                break;
            case '?':
            default:

                return 1;
        }

    if (matrix_size <= 0 and fermions <= 0){
        cerr << "Matrix size and fermions but be greater than zero.";
        exit(1);
    }

    auto m = new Model(matrix_size, fermions, alpha, beta);
    m->run();

    return 0;
}