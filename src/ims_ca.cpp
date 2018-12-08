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
    int c, matrix_size = 0,  fermions = 0;
    double alpha = 0.5, beta = 10, j_s = 2, j_d = 10;

    while ((c = getopt(argc, argv, "a:b:m:f:s:d:")) != -1)
        switch (c) {
            case 'a': // alpha
                alpha = atof(optarg);
                break;
            case 'b': // beta
                beta = atof(optarg);
                break;
            case 'm': // matrix edge size
                matrix_size = atoi(optarg);
                break;
            case 'f': // count of fermions
                fermions = atoi(optarg);
                break;
            case 's': // J-s
                j_s = atof(optarg);
                break;
            case 'd': // J-d
                j_d = atof(optarg);
                break;
            case '?':
            default:

                return 1;
        }

    if (matrix_size <= 0 and fermions <= 0){
        cerr << "Matrix size and fermions but be greater than zero.";
        exit(1);
    }

    auto m = new Model(matrix_size, fermions, alpha, beta, j_s, j_d);
    m->run();

    return 0;
}