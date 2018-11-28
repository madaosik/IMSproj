/*********************************
 * IMS - cellular automata       *
 * 2018/2019                     *
 * Tomas Willaschek  (xwilla00)  *
 * Adam Lanicek      (xlanic04)  *
 *********************************/

#ifndef IMS_MODEL_H
#define IMS_MODEL_H

#include <iostream>
#include <map>
#include <vector>

using std::map;
using std::pair;
using std::vector;

class Model {
public:
    Model(int, int, double, double, double, double);
    ~Model();

    /**
     * Prints matrix to stdout
     */
    void print_matrix();
    /**
     * Prints s_arr to stdout
     */
    void print_s_arr();
    /**
     * Starts simulations
     */
    void run();

private:
    /** Input arguments */
    double alpha;
    double beta;
    double j_s;
    double j_d;
    int fermions;
    int edge;
    int matrix_size; //! edge * edge


    /** Program defined variables **/
    int iteration; //! iteration count
    int exit_pos; //! position on exit on map
    int **matrix; //! [cols][rows]
    double **s_arr; //! [cols][rows]

    //! dynamic array
    //! index in map: iterations
    map< int, vector<int>* > *d_arr;

    //! fermion ID -> coordinates
    map< int, pair<int, int> > *fermions_pos;
    //! fermion ID -> coordinates
    //! contains fermions position one iteration backward
    map< int, pair<int, int> > *fermions_pos2;




    /**
     * Generates random number from zero to range
     * @param range
     * @return random number
     */
    int random(int);

    /**
     * Generates fermions to random position into matrix
     * save their position to fermions_pos
     */
    void populate_matrix();

    /**
     * Populate each index from array with value
     * Indexes close to exit have bigger value than others
     */
    void generate_s_arr();

    /**
     * Insert value to d_arr -> < matrix_index < iteration, value > >
     * @param row: index in matrix
     * @param col: index in matrix
     */
    void insert(int, int);

    void perform_step();

    void get_limits(int, int &, int &);
};

#endif //IMS_MODEL_H
