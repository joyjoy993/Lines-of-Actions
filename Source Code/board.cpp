#include "board.h"

Board::Board()
{
    int dir[8][2] = {{0,-1}, {0,1}, {-1,0}, {1,0}, {-1,-1}, {1,1}, {1,-1}, {-1,1}};
    for(int i = 0; i<8; i++)
        for(int j = 0; j<2; j++)
            dir_move[i][j] = dir[i][j];
}

//////////////////////////////////////////////
/// \brief move a checker
/// \param m: a check board
/// \param x: coordinate x
/// \param y: coordinate y
/// \param dir: a specific direction
/// \param black: current # of black checkers
/// \param white: current # of white checkers
/// \return true/false
///
bool Board::move_checker(node (&m)[6][6], int x, int y, int dir, int &black, int &white)
{
    if(!move_valid(m, dir, x, y)) return false;

    int dx = x + get_dir_number(m, dir, x, y) * dir_move[dir][0];
    int dy = y + get_dir_number(m, dir, x, y) * dir_move[dir][1];
    int type = m[x][y].type;
    m[x][y].type = empty_type;
    if(m[dx][dy].type != empty_type)// captured
    {
        if(m[dx][dy].type == machine_type) black--;
        else white--;
    }
    m[dx][dy].type = type;
    return true;
}

//////////////////////////////////////////////
/// \brief check if it's a valid move
/// \param m: a check board
/// \param dir: a specific direction
/// \param x: coordinate x
/// \param y: coordinate y
/// \return true/false
///
bool Board::move_valid(node m[6][6], int dir, int x, int y)
{
    if(m[x][y].type == empty_type) return false;
        int moves = get_dir_number(m, dir, x, y);
        int tx = x + dir_move[dir][0]*moves;
        int ty = y + dir_move[dir][1]*moves;

        int type = m[x][y].type;
        if(!check_vaild(tx, ty))
            return false;
        else
        {
            tx = x;
            ty = y;
            for(int j = 0; j<moves; j++)
            {
                tx += dir_move[dir][0];
                ty += dir_move[dir][1];
                if(j!=moves-1 && m[tx][ty].type != type && m[tx][ty].type!=empty_type) return false;
                if(j==moves-1 && m[tx][ty].type == type) return false;
            }
        }
        return true;
}

//////////////////////////////////////////////
/// \brief check if this coordinate is valid, like (9,4), it's over the board
/// \param x: coordinate x
/// \param y: coordinate y
/// \return true/false
///
bool Board::check_vaild(int x, int y)
{
    return !(x>=board_size||x<0||y>=board_size||y<0);
}

//////////////////////////////////////////////
/// \brief check if a player is win
/// \param m: a check board
/// \param type: 0 for black, 1 for white
/// \param black: current # of black checkers
/// \param white: current # of white checkers
/// \param connectnums: # of connected checker
/// \return true/false
///
bool Board::win(node m[6][6], int type, int black, int white, int &connectnums)
{
    if(type == machine_type && black == 1) return true;
    if(type == human_type && white == 1) return true;
    int visit[6][6] = {0};
    for(int i = 0; i<board_size; i++)
        for(int j = 0; j<board_size; j++)
            if(m[i][j].type == type)
            {
                int count = 0;
                queue<node> q;
                q.push(m[i][j]);
                visit[i][j] = 1;
                while(!q.empty())
                {
                    node n = q.front();
                    q.pop();
                    count++;
                    for(int k = 0; k<8; k++)
                    {
                        int dx = n.x + dir_move[k][0];
                        int dy = n.y + dir_move[k][1];
                        if(check_vaild(dx, dy))
                        {
                            if(m[dx][dy].type == type && !visit[dx][dy])
                            {
                                q.push(m[dx][dy]);
                                visit[dx][dy] = 1;
                            }
                        }
                    }
                }

                if(type == machine_type && count == black) return true;
                if(type == human_type && count == white) return true;

                if(count > connectnums) connectnums = count;
                if(type == machine_type && connectnums*2 >= black) return false; // no need to be continued
                if(type == human_type && connectnums*2 >= white) return false;
            }
    return false;
}
//////////////////////////////////////////////
/// \brief Board::get # of checkers on a direction for a specific (x,y)
/// \param m: a check board
/// \param dir: a specific direction
/// \param x: coordinate x
/// \param y: coordinate y
/// \return # of checkers on a direction
///
int Board::get_dir_number(node m[6][6], int dir, int x, int y)
{
    int tx, ty;
    int type = m[x][y].type;
    if(type == empty_type) return 0;//empty node
    int d = dir % 2;
    int sum = 1;
    if(d == 0) { d = dir + 1; }
    else { d = dir; dir--; }

    for(int k = dir; k<=d; k++)
    {
        tx = x + dir_move[k][0];
        ty = y + dir_move[k][1];
        int count = 0;
        while(check_vaild(tx, ty))
        {
            if(m[tx][ty].type != empty_type) count++;
            tx = tx + dir_move[k][0];
            ty = ty + dir_move[k][1];
        }
        sum += count;
    }
    return sum;
}
