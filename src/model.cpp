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

Model::Model(int matrix_size, int fermions, double alpha, double beta, double j_s, double j_d) {
    this->fermions = fermions;
    this->alpha = alpha;
    this->beta = beta;
    this->edge = matrix_size;
    this->matrix_size = matrix_size * matrix_size;
    this->j_d = j_d;
    this->j_s = j_s;

    // set exit to center of edge
    this->exit_pos = edge / 2;

    this->matrix = new int*[matrix_size];
    this->s_arr = new double*[matrix_size];
    for(int i = 0; i < matrix_size; i++){
        this->matrix[i] = new int[matrix_size];
        this->s_arr[i] = new double[matrix_size];
    }
    d_arr = new map<int, vector<int>* >;
    fermions_pos = new map<int, pair<int, int> >;
    fermions_pos2 = new map<int, pair<int, int> >;

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
    delete fermions_pos2;
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

    if((d_arr->find(index)) == d_arr->end()) {
        auto tmp = new vector<int>;
        tmp->push_back(iteration);
        d_arr->insert(make_pair(index, tmp));
    }
    else{
        d_arr->operator[](index)->push_back(iteration);
    }
}

void Model::run() {
    print_matrix();
    for(int i = 0; i < 10; ++i){
        perform_step();
    }
}

void Model::perform_step() {
    int row_start, col_start, row_end, col_end;// start row/col index to check move
             // end row/col index

    double preference_matrix[3][3] = {{1.0/6, 1.0/9, 2.0/18},
                                      {1.0/6, 1.0/9, 2.0/18},
                                      {1.0/6, 1.0/9, 2.0/18}};
    double tmp_matrix[3][3];

    int move_matrix[edge][edge];
    std::fill(move_matrix[0], move_matrix[0] + matrix_size, 0);
    // calculate new position and save it into data
    map< int, pair<int, int> > data;
    for(int i = 0; i < fermions; ++i){
        tmp_matrix[3][3] = {0};
        std::fill(tmp_matrix[0], tmp_matrix[0] + 9, 0);

        // current position of processed fermion
        auto position = fermions_pos->find(i)->second;
        // s bosons in current position
        double ts_xy = s_arr[position.first][position.second];
        // support for indexing
        int index = position.first * edge + position.second;
        // d bosons in current position
        double td_xy = 0;
        if ((d_arr->find(index)) != d_arr->end())
            td_xy = d_arr->operator[](index)->size();
        // delta s(i,j)
        double delta_s_ij;
        // delta d(i,j)
        double delta_d_ij;
        // tmp for counting result
        double tmp1, tmp2;

        int d_ij;
        double normalization_counter = 0;
        int not_null_coordinates = 0;

        // count available moves
        get_limits(position.first, row_start, row_end);
        get_limits(position.second, col_start, col_end);

        for(int x = row_start; x <= row_end; ++x){
            for(int y = col_start; y <= col_end; ++y){
                // check if fermion can access this position -- substitute n-i,j
                if(matrix[position.first + x][position.second + y] == 0){
                    delta_s_ij = (s_arr[position.first + x][position.second + y]) - ts_xy;
                    index = (position.first + x) * edge + position.second + y;
                    if ((d_arr->find(index)) != d_arr->end()) // FIXME: this is always ZEROOOOOO
                        delta_d_ij = d_arr->operator[](index)->size() - td_xy;
                    else
                        delta_d_ij = 0;
                    if (delta_d_ij < 0) // TODO: check
                        delta_d_ij = 0;
                    tmp1 = Exponential(beta * j_s * delta_s_ij);
                    tmp2 = Exponential(beta * j_d * delta_d_ij);
                    d_ij = 1; // FIXME: this should be counted
                    double val = preference_matrix[x + 1][y + 1] * tmp1 * tmp2 * d_ij;
                    tmp_matrix[x+1][y+1] = val;
                    normalization_counter += val;
                    if(val)
                        ++not_null_coordinates;
                }
            }
        }
        // TODO: calculate normalization and normalize 'random' or each value in matrix

        // select random position for move
        double random = Random();
        double tmp_num = 0;
        for(int x = row_start; x <= row_end; ++x) {
            for (int y = col_start; y <= col_end; ++y) {
                // TODO: choose random move and save its coordindates [x,y] increment position in move_matrix
            }
        }
    }

    // TODO: perform moves --> iterate over data and resolve conflicts from move_matrix


    ++iteration;
}

void Model::get_limits(int position, int &start, int &end) {
    if (position == 0){
        start = 0;
        end = 1;
    }
    else if(position == edge - 1){
        start = -1;
        end = 0;
    }
    else{
        start = -1;
        end = 1;
    }
}