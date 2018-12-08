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
    /**
     * Constructor
     * @param matrix_size: size of matrix edge
     * @param fermions: total count of fermions generated on map
     * @param alpha: alpha value
     * @param beta: beta value
     * @param j_s: J-s value
     * @param j_d: J-d value
     */
    Model(int, int, double, double, double, double, double);
    ~Model();

    /**
     * Prints matrix to stdout
     * debug func
     */
    void print_matrix();
    /**
     * Prints s_arr to stdout
     * debug func
     */
    void print_s_arr();
    /**
     * Prints d_arr to stdout
     * debug func
     */
    void print_d_arr();
    /**
     * Starts simulations
     */
    int run();

private:
    /** Input arguments */
    double alpha;
    double beta;
    double j_s;
    double j_d;
    double j_o;
    int fermions;
    int edge;
    int matrix_size; //! edge * edge


    /** Program defined variables **/
    int iteration; //! iteration count
    int exit_pos; //! position on exit on map
    int **matrix; //! [cols][rows]
    double **s_arr; //! [cols][rows]
    int finished_fermions;

    //! dynamic array
    //! index in map: iterations
    map< int, vector<int>* > *d_arr;

    //! fermion ID -> coordinates
    map< int, pair<int, int> > *fermions_pos;
    //! fermion ID -> coordinates
    //! contains fermions last position
    map< int, pair<int, int> > *old_fermions_pos;

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
     * Insert d-bosson (value to d_arr)
     * < matrix_index < iteration > >
     * @param row: index in matrix
     * @param col: index in matrix
     */
    void insert(int, int, pair< int, int >);

    /**
     * Perform one iteration with all needed steps except removing old D-bossons
     * * count probability for all 9 coordinates where fermion can access
     * * select randomly one of them
     * * resolve conflicts - two or more fermions want to go on same position
     * * update fermion values and maps
     */
    void perform_step();

    /**
     * Gets limits for current 3x3 matrix
     * Positions where current fermion can access
     * @param position: coordinate -> whether is counted for X or Y coordinate
     * @param start: reference to start variable where is result stored
     * @param end: reference to end variable where is result stored
     */
    void get_limits(int, int &, int &);

    /**
     * Remove d bosons oder than iteration - 2 cycles with alpha probability
     */
    void remove_old_d_bossons();

    /**
     * Save old coordinates to old_fermions_pos before performing move
     * @param id: fermion ID
     * @param fermion_pos: fermion current position
     */
    void update_fermion_old_position(int, pair<int, int>);

    /**
     * Compute probability of direction for index
     * @param id: fermion ID
     * @param position: fermions current position
     * @param x: currently counted move row index
     * @param y: currently counter move col index
     * @return exp(-beta * j_d) if computed coordinates are same as fermion moved from
     *         exp(beta * j_o) if coordinates continue in same direction as last move
     *         1.0 else
     */
    double compute_direction(int, pair<int, int>, int, int);
};

#endif //IMS_MODEL_H
