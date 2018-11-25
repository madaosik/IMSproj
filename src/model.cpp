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

    d_arr = new map<int, map<int, int> >;
    fermions_pos = new map<int, pair<int, int> >;

    populate_matrix();
    generate_s_arr();
}

Model::~Model() {
    for(int i = 0; i < matrix_size; i++){
        delete matrix[i];
        delete s_arr[i];
    }
    delete matrix;
    delete s_arr;
    delete d_arr;
    delete fermions_pos;
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
        fermions_pos->insert(make_pair(i, make_pair(row, column)));
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

void Model::insert(int row, int col) {
    int index = row * edge + col;

    if((d_arr->find(index)) == d_arr->end()){
        map<int, int> tmp;
        tmp.insert(make_pair(iteration, 1));
        d_arr->insert(make_pair(index, tmp));
    }
    else
        d_arr->operator[](index)[iteration] = 1;
}

void Model::run() {
    print_matrix();
    for(int i = 0; i < 10; ++i){
        perform_step();
    }
}

void Model::perform_step() {
    int row_start, col_start, // start row/col index to check move
        row_end, col_end;   // end row/col index

    double preference_matrix[9] = {1.0/6, 1.0/9, 2.0/18,
                                   1.0/6, 1.0/9, 2.0/18,
                                   1.0/6, 1.0/9, 2.0/18};
    double move_matrix[9];

    // calculate new position and save it into data
    map< int, pair<int, int> > data;
    for(int i = 0; i < fermions; ++i){
        // current position of processed fermion
        auto position = fermions_pos->find(i)->second;

        // count available moves
        get_limits(position.first, row_start, row_end);
        get_limits(position.second, col_start, col_end);

        // TODO: zkontrolovat s kolizama z matrix

    }
}

void Model::get_limits(int position, int &start, int &end) {
    if (position == 0){
        start = 0;
        end = 1;
    }
    else if(position == edge - 1){
        start = position - 1;
        end = position;
    }
    else{
        start = position - 1;
        end = position + 1;
    }
}