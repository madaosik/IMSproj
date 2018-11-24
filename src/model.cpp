/*********************************
 * IMS - cellular automata       *
 * 2018/2019                     *
 * Tomas Willaschek  (xwilla00)  *
 * Adam Lanicek      (xlanic04)  *
 *********************************/
#include "model.h"

#include <iostream>
#include <cstdlib>

using std::cout;
using std::endl;

Model::Model(int matrix_size, int fermions, float alpha, float beta) {
    this->matrix_size = matrix_size;
    this->fermions = fermions;
    this->alpha = alpha;
    this->beta = beta;

    int size = matrix_size * matrix_size;
    this->matrix = (int *) malloc(size * sizeof(int));
    this->d_arr = (float *) malloc(size * sizeof(float));
    this->s_arr = (float *) malloc(size * sizeof(float));
}

Model::~Model() {
    free(matrix);
    free(d_arr);
    free(s_arr);
}

void Model::get_matrix_size() {
    cout << "Matrix is " << matrix_size << "x" << matrix_size << endl;
}