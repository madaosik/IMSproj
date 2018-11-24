/*********************************
 * IMS - cellular automata       *
 * 2018/2019                     *
 * Tomas Willaschek  (xwilla00)  *
 * Adam Lanicek      (xlanic04)  *
 *********************************/

#ifndef IMS_MODEL_H
#define IMS_MODEL_H

#include <iostream>
#include <vector>

using std::vector;

class Model {
public:
    Model(int matrix_size, int fermions, float alpha, float beta);
    ~Model();
    void get_matrix_size();
private:
    float alpha;
    float beta;
    int fermions;
    int matrix_size;

    int *matrix;
    float *s_arr;
    float *d_arr;
};


#endif //IMS_MODEL_H
