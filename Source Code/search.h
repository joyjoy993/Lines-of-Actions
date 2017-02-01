#ifndef SEARCH_H
#define SEARCH_H
#include <QThread>
#include <board.h>

class Search : public Board, public QThread
{
public:
    Search();
    Search(node map[6][6], int b, int w, int d, int time, int machine, int human, int b_size, int l);//initial
    int getcom(node m[6][6], int type, int &x, int &y, int black, int white);//get the centre-of-mass of the board
    int eval(node m[6][6], int type, int black, int white);//evaluate a state
    int min_v(node m[6][6], int a, int b, int depth, int black, int white);//min function
    int max_v(node m[6][6], int a, int b, int depth, int black, int white);//max function
    int absearch(node m[6][6], int black, int white);//main alpha beta search function
    bool check_depth_zero(node m[6][6]);//check whether win in only one move

    bool finish = false;
    int max_depth = 1;//maximum depth of game tree reached (let the root node be level 0)
    int expanded_node = 1;//total number of nodes generated (including root node) in the tree
    int max_time = 0;//number of times the evaluation function was called within the MAX-VALUE function
    int min_time = 0;//number of times the evaluation function was called within the MIN-VALUE function
    int max_pruning = 0;//number of times pruning occurred within the MAX-VALUE function
    int min_pruning = 0;//number of times pruning occurred within the MIN-VALUE function.
    int val_return = 0;
    int level = 1;
    vector<action> max_action;//memorize the first level actions

private:
    void run();//overload run() of QTheard
};

#endif // SEARCH_H
