/*********************************
 * IMS - cellular automata       *
 * 2018/2019                     *
 * Tomas Willaschek  (xwilla00)  *
 * Adam Lanicek      (xlanic04)  *
 *********************************/
#include "model.h"

#include <bits/stdc++.h>
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
    this->finished_fermions = 0;

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

    populate_matrix();
    generate_s_arr();

    srand(time(NULL));
    RandomSeed(rand());
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
    return (int)(Random() * max);
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
        if(i == exit_pos || i == exit_pos + 1 || i == exit_pos - 1)
            cout << " ";
        else
            cout << "|";
        for(int j = 0; j < edge; j++){
            if(matrix[i][j] > 1)
                cout << "Error" << endl;
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

void Model::print_d_arr() {
    for(int i = 0; i < matrix_size; ++i){
        if (i % edge == 0)
            cout << endl;
        auto val = d_arr->find(i);
        if (val != d_arr->end())
            cout << val->second->size() << " ";
        else
            cout << ". ";
    }
    cout << endl;
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

void Model::insert(int row, int col, pair< int, int > ferm) {
    if(row == ferm.first && col == ferm.second)
        return;

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

void Model::perform_step() {
    ++iteration;

    int row_start, col_start, row_end, col_end;// start row/col index to check move, end row/col index

    double preference_matrix[3][3] = {{1.0/6, 1.0/9, 2.0/18},
                                      {1.0/6, 1.0/9, 2.0/18},
                                      {1.0/6, 1.0/9, 2.0/18}};
    double tmp_matrix[3][3];

    int move_matrix[edge][edge];
    std::fill(move_matrix[0], move_matrix[0] + matrix_size, 0);

    // calculate new position and save it into data
    map< int, pair<int, int> > data;

    for(int i = 0; i < fermions; ++i){
        auto f = fermions_pos->find(i);
        if(f == fermions_pos->end())
            continue;

        // current position of processed fermion
        auto position = f->second;

        // if fermion is on end position, remove it
        if(position.second == 0){
            if(position.first == exit_pos || position.first == exit_pos + 1 || position.first == exit_pos - 1 ||
               position.first == exit_pos + 2 || position.first == exit_pos - 2){
                ++finished_fermions;
                fermions_pos->erase(i);
                continue;
            }
        }

        tmp_matrix[3][3] = {0};
        std::fill(tmp_matrix[0], tmp_matrix[0] + 9, 0);

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
        double normalization = 0;

        // count available moves
        get_limits(position.first, row_start, row_end);
        get_limits(position.second, col_start, col_end);

        // fill tmp_matrix
        for(int x = row_start; x <= row_end; ++x){
            for(int y = col_start; y <= col_end; ++y){
                // check if fermion can access this position -- substitute n-i,j
                if(matrix[position.first + x][position.second + y] == 0 || (x == 0 && y == 0)){
                    delta_s_ij = (s_arr[position.first + x][position.second + y]) - ts_xy;
                    index = (position.first + x) * edge + position.second + y;
                    if ((d_arr->find(index)) != d_arr->end())
                        delta_d_ij = d_arr->operator[](index)->size() - td_xy;
                    else
                        delta_d_ij = 0;
                    tmp1 = exp(beta * j_s * delta_s_ij);
                    tmp2 = exp(beta * j_d * delta_d_ij);
                    d_ij = 1; // FIXME: this should be counted
                    double val = preference_matrix[x + 1][y + 1] * tmp1 * tmp2 * d_ij;
                    tmp_matrix[x+1][y+1] = val;
                    normalization += val;
                }
            }
        }

        // select random position for move
        double random = Random() * normalization;
        bool stop_iteration = false;
        for(int x = 2; x >= 0; --x) {
            for (int y = 2; y >= 0; --y) {
                normalization -= tmp_matrix[x][y];
                // check if current index is the one && index is not zero (fermion can not access this position)
                if(normalization < random && (tmp_matrix[x][y] != 0 || (x == 0 && y == 0))){
                    stop_iteration = true;
                    ++move_matrix[position.first + x - 1][position.second + y - 1];
                    data.insert(make_pair(i, make_pair(position.first + x - 1, position.second + y - 1)));
                    break;
                }
            }
            if(stop_iteration)
                break;
        }
        if(!stop_iteration)
            cout << "Error 1" << endl;
    }

    // do move
    for(int i = 0; i < fermions; ++i){
        auto pos = data.find(i);
        if(pos == data.end())
            continue;
        auto position = pos->second;

        // if fermione is alone, make move
        if(move_matrix[position.first][position.second] == 1){
            auto ferm = fermions_pos->find(i);
            if(ferm == fermions_pos->end()){
                cout << "Error 2" << endl;
                continue;
            }
            insert(position.first, position.second, ferm->second);
            ferm->second = make_pair(position.first, position.second);
        }
        else{
            double random = Random() * move_matrix[position.first][position.second];
            auto ferm = fermions_pos->find(i);

            // fermion won battle for position
            if(random < 1.0){
                ferm->second = make_pair(position.first, position.second);
                insert(position.first, position.second, ferm->second);

                for(int j = i + 1; j < fermions; ++j){
                    auto ferm_dupl = data.find(j);
                    if(ferm_dupl == data.end())
                        continue;

                    if(ferm_dupl->second.first == position.first && ferm_dupl->second.second == position.second){
                        ferm_dupl = fermions_pos->find(j);
                        if(ferm_dupl == data.end()){
                            cout << "Error 3" << endl;
                            continue;
                        }
                        ++move_matrix[ferm_dupl->second.first][ferm_dupl->second.second];
                        --move_matrix[position.first][position.second];
                        data.erase(j);
                    }
                }
            }
            else{
                ++move_matrix[ferm->second.first][ferm->second.second];
                --move_matrix[position.first][position.second];
                insert(position.first, position.second, ferm->second);
            }
        }
    }

    for(int i = 0; i < edge; ++i)
        for(int j = 0; j < edge; ++j)
            matrix[i][j] = move_matrix[i][j];
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

void Model::remove_old_d_bossons() {
    int iteration = this->iteration - 2;
    if (iteration > 0){
        for (int i = 0; i < matrix_size; ++i) {
            auto boson_item = d_arr->find(i);
            if (boson_item == d_arr->end())
                continue;
            if (Random() < alpha && !boson_item->second->empty())
                boson_item->second->erase(boson_item->second->begin());
        }
    }
}

void Model::run() {
//    print_s_arr();
    print_matrix();
    while(finished_fermions != fermions){
        perform_step();
        remove_old_d_bossons();
//        print_matrix();
//        print_d_arr();
    }
    cout << "Total count of iterations is: " << iteration << endl;
}

