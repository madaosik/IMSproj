/*********************************
 * IMS - cellular automata       *
 * 2018/2019                     *
 * Tomas Willaschek  (xwilla00)  *
 * Adam Lanicek      (xlanic04)  *
 *********************************/
#include "model.h"


#include <iostream>
#include <cstdlib>
#include <simlib.h>
#include <ctime>
#include <iomanip>

using std::cout;
using std::endl;
using std::pair;
using std::make_pair;

Model::Model(int matrix_size, int fermions, float alpha, float beta) {
    this->fermions = fermions;
    this->alpha = alpha;
    this->beta = beta;
    this->edge = matrix_size;
    this->matrix_size = matrix_size * matrix_size;

    // set exit to center of edge
    this->exit_pos = edge / 2;

    this->matrix = new int*[matrix_size];
    this->s_arr = new double*[matrix_size];
    for(int i = 0; i < matrix_size; i++){
        this->matrix[i] = new int[matrix_size];
        this->s_arr[i] = new double[matrix_size];
    }

    populate_matrix();
    print_matrix();
    generate_s_arr();
    print_s_arr();
}

Model::~Model() {
    // TODO free matrixes
}

int Model::random(int max) {
    srand(time(NULL));
    return rand() % max;
}

void Model::populate_matrix() {
    int row, column, num;
    for(int i = 0; i < fermions; ++i){
        num = random(matrix_size);
        row = num / edge;
        column = num % edge;
        if(matrix[row][column]){
            --i;
            continue;
        }
        matrix[row][column] = 1;
    }
}

void Model::print_matrix() {
    for(int i = 0; i < edge + 1; ++i)
        cout << "--";
    cout << "-" << endl;
    for(int i = 0; i < edge; ++i){
        if(i == exit_pos)
            cout << " ";
        else
            cout << "|";
        for(int j = 0; j < edge; j++){
            if(matrix[i][j])
                cout << " x";
            else
                cout << " .";
        }
        cout << " |" << endl;
    }
    for(int i = 0; i < edge + 1; ++i)
        cout << "--";
    cout << "-" << endl;
}

void Model::print_s_arr(){
    for(int i = 0; i < edge; ++i)
        cout << "--------";
    cout << "---" << endl;
    for(int i = 0; i < edge; ++i){
        if(i == exit_pos)
            cout << "  ";
        else
            cout << "| ";
        for(int j = 0; j < edge; j++){
            cout << std::setprecision(4) << std::fixed << s_arr[i][j] << "  ";
        }
        cout << "|" << endl;
    }
    for(int i = 0; i < edge; ++i)
        cout << "--------";
    cout << "---" << endl << endl;
}

void Model::generate_s_arr() {
    double step = 1.0 / (2.0 * edge), result;

    for(int i = 0; i < edge; ++i){
        if(i < exit_pos)
            result = 1 - step * (exit_pos - i);
        else
            result = 1 - step * (i - exit_pos);

        for(int j = 0; j < edge; j++){
            s_arr[i][j] = result + (1 - j * step) - 1;
        }
    }
}

void Model::insert(int row, int col, int value) {
    int index = row * edge + col;
    map<int, map<int, int>>::iterator it;

    if((it = d_arr.find(index)) == d_arr.end()){
        map<int, int> tmp;
        tmp.insert(make_pair(iteration, value));
        d_arr.insert(make_pair(index, tmp));
    }
    else
        it->second.insert(make_pair(iteration, value));

}

void Model::run() {
    map<int, map<int, int>>::iterator it;
    map<int, int>::iterator it2;

    insert(1,1,1);

    insert(1,1,2);
    insert(1,1,3);
    insert(1,1,4);

    it = d_arr.find(12);
    for(it2 = it->second.begin(); it2 != it->second.end(); it++)
        cout << it2->second << endl;


    bool a = it == d_arr.end();


    a = it->second.find(1) == it->second.end();
    a = it->second.find(2) == it->second.end();
    a = it->second.find(3) == it->second.end();
    a = it->second.find(4) == it->second.end();
    a = it->second.find(5) == it->second.end();
}

