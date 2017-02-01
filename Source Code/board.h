#ifndef BOARD_H
#define BOARD_H
#include <QMainWindow>
#include <QLabel>
#include <QDebug>
#include <queue>
#include <time.h>
#include <math.h>
#include <vector>
#include <iostream>
#include <QObject>
#include <checker.h>
#include <QMessageBox>
#include <QThread>
#include <QTimer>

using namespace std;

struct node{
    int dir[8];
    int type;
    int x;
    int y;
    node(int t, int xt, int yt)
    {
        type = t;
        x = xt;
        y = yt;
    }
    node()
    {
        type = 0;
        x = 0;
        y = 0;
    }
    node& operator=(const node& n)//overload
    {
        for(int i = 0; i<8; i++) dir[i] = n.dir[i];
        type = n.type;
        x = n.x;
        y = n.y;
        return *this;
    }
};//struct of a checker

struct action{
    int val;
    int dir;
    int x;
    int y;
    action(int v, int d, int tx, int ty)
    {
        val = v;
        dir = d;
        x = tx;
        y = ty;
    }
};//struct of an action

class Board
{
    //Q_OBJECT
public:
    Board();
    node map_loa[6][6];//board map
    time_t clockBegin, clockEnd;//memorize the begin time and end time
    int dir_move[8][2];//{x,y}, coordinates of 8 directions
    // dir: (0,1,2,3):(up,down,left,right); (4,6,6,7):(left-top,right-bottom,right-top,left-bottom)
    int black = 0;//number of black
    int white = 0;//number of white
    int difficulty = 0;//control difficulty according to the depth level
    int time_limit = 0;//10s
    int machine_type = 0;//0-black, 1-white
    int human_type = 1;//0-black, 1-white
    int empty_type = 2;//always is 2
    int board_size = 5;//board size, 5 or 6

    bool move_checker(node (&m)[6][6], int x, int y, int dir, int &black, int &white);//move a checker
    bool check_vaild(int x, int y);//check if this coordinate is valid, like (9,4), it's over the board
    bool move_valid(node m[6][6], int dir, int x, int y);//check if it's a valid move
    bool win(node m[6][6], int type, int black, int white, int &connectnums);//check if a player is win
    int get_dir_number(node m[6][6], int dir, int x, int y);//get # of checkers on a direction for a specific (x,y)
};

#endif // BOARD_H
