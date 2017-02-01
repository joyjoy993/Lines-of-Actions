#include "search.h"

Search::Search()
{

}

//////////////////////////////////////////////
/// \brief Search::Search
/// \param map: a check board
/// \param b: current # of black checkers
/// \param w: current # of white checkers
/// \param d: difficulty
/// \param time: time limit
/// \param machine: machine type
/// \param human: human type
/// \param b_size: board size
/// \param l: level
Search::Search(node map[6][6], int b, int w, int d, int time, int machine, int human, int b_size, int l)
{
    for(int i = 0; i<b_size; i++)
        for(int j = 0; j<b_size; j++)
        map_loa[i][j] = map[i][j];
    black = b;
    white = w;
    difficulty = d;
    time_limit = time;
    machine_type = machine;
    human_type = human;
    board_size = b_size;
    level = l;
}

//////////////////////////////////////////////
/// \brief overload run() of QTheard
///
void Search::run()
{
    //qDebug()<<"start thread"<<endl;
    clockBegin = time(NULL);
    absearch(map_loa, black, white);
    finish = true;
    //qDebug()<<"end thread"<<endl;
}

//////////////////////////////////////////////
/// \brief get the centre-of-mass of the board
/// \param m: a check board
/// \param type: checker type
/// \param x: coordinate x
/// \param y: coordinate y
/// \param black: current # of black checkers
/// \param white: current # of white checkers
/// \return (x,y) of the centre-of-mass of the board
///
int Search::getcom(node m[6][6], int type, int &x, int &y, int black, int white)
{
    for(int i = 0; i<board_size; i++)
    for(int j = 0; j<board_size; j++)
    {
        if(m[i][j].type == type)//black
        {
            x += i;
            y += j;
        }
    }
    if(type)
    {
        x /= white;
        y /= white;
    }
    else
    {
        x /= black;
        y /= black;
    }
    return 0;
}

//////////////////////////////////////////////
/// \brief evaluate a state
/// \param m: a check board
/// \param type: checker type
/// \param black: current # of black checkers
/// \param white: current # of white checkers
/// \param connectnums: # of checkers connected together
/// \return evaluation value of a state
///
int Search::eval(node m[6][6], int type, int black, int white)
{
    double comrate = 0;
    int avgx = 0;
    int avgy = 0;
    getcom(m, type, avgx, avgy, black, white);

    for(int i = 0; i<board_size; i++)
    for(int j = 0; j<board_size; j++)
    {
        if(m[i][j].type == type)
        {
            double x = abs(i - avgx);
            double y = abs(j - avgy);
            if(level >=2 )
                comrate += (x + y);
            else
                comrate += sqrt(pow(x, 2) + pow(y, 2));
        }
    }
    comrate *= 1000;
    if(level == 3)
    {
        if(type) { comrate /= white; }
        else { comrate /= black; }
    }

    return comrate*4;
}

//////////////////////////////////////////////
/// \brief min function
/// \param m: a check board
/// \param a: alpha value
/// \param b: beta value
/// \param depth: current depth
/// \param black: current # of black checkers
/// \param white: current # of white checkers
/// \return val
///
int Search::min_v(node m[6][6], int a, int b, int depth, int black, int white)
{
    int connectnums = 1;
    int val = 0;
    int v = 100000;
    bool flag = true;//draw

    if(win(m, machine_type, black, white, connectnums)) return 100000;
    if(win(m, human_type, black, white, connectnums)) return -100000;

    clockEnd = time(NULL);

    if(clockEnd - clockBegin >= time_limit || depth >= difficulty)
    {
        min_time++;
        val = eval(m, machine_type, black, white);
        val = eval(m, human_type, black, white) - val;
        return val;
    }

    if(depth > max_depth) max_depth = depth;

    for(int i = 0; i<board_size; i++)
    for(int j = 0; j<board_size; j++)
    {
        if(m[i][j].type == human_type)
        {
            for(int f = 0; f<8; f++)
            {
                node t[6][6];
                for(int k = 0; k<board_size; k++)
                for(int l = 0; l<board_size; l++)
                    t[k][l] = m[k][l];
                int bl = black;
                int wh = white;
                if(move_checker(t, i, j, f, bl, wh))
                {
                    flag = false;//can move
                    expanded_node++;
                    v = min(v, max_v(t, a, b, depth+1, bl, wh));

                    if(v<=a) { min_pruning++; return v; }
                    b = min(b, v);
                }
            }
        }
    }
    if(flag) { return 100000; }//loose
    return v;
}

//////////////////////////////////////////////
/// \brief max function
/// \param m: a check board
/// \param a: alpha value
/// \param b: beta value
/// \param depth: current depth
/// \param black: current # of black checkers
/// \param white: current # of white checkers
/// \return val
///
int Search::max_v(node m[6][6], int a, int b, int depth, int black, int white)
{
    int connectnums = 1;
    int val = 0;
    int v = -100000;
    bool flag = true;

    if(win(m, machine_type, black, white, connectnums)) return 100000;
    if(win(m, human_type, black, white, connectnums)) return -100000;

    clockEnd = time(NULL);
    if(clockEnd - clockBegin >= time_limit || depth >= difficulty)
    {
        max_time++;
        val = eval(m, machine_type, black, white);
        val = eval(m, human_type, black, white) - val;
        return val;
    }

    if(depth > max_depth) max_depth = depth;
    for(int i = 0; i<board_size; i++)
    for(int j = 0; j<board_size; j++)
    {
        if(m[i][j].type == machine_type)
        {
            for(int f = 0; f<8; f++)
            {
                node t[6][6];
                for(int k = 0; k<board_size; k++)
                for(int l = 0; l<board_size; l++)
                    t[k][l] = m[k][l];
                int bl = black;
                int wh = white;
                if(move_checker(t, i, j, f, bl, wh))
                {
                    flag = false;
                    expanded_node++;
                    v = max(v, min_v(t, a, b, depth+1, bl, wh));

                    if(depth == 0)
                    {
                        action ac(v, f, i, j);
                        max_action.push_back(ac);
                    }
                    if(v>=b) { max_pruning++; return v; }
                    a = max(a, v);
                }
            }
        }
    }
    if(flag) return -100000;//loose
    return v;
}

//////////////////////////////////////////////
/// \brief define a less function for vector sort
/// \param s1
/// \param s2
/// \return
///
bool Less(const action& s1, const action& s2)
{
        return s1.val > s2.val;
}

//////////////////////////////////////////////
/// \brief main alpha beta search function
/// \param m: a checker board
/// \param black: current # of black checkers
/// \param white: current # of white checkers
/// \return val
///
int Search::absearch(node m[6][6], int black, int white)
{
    int v;
    if(!check_depth_zero(m))
    {
        v = max_v(m, -100000, 100000, 0, black, white);

        sort(max_action.begin(), max_action.end(), Less);

        //for(int j = 0; j<max_action.size(); j++)
        //    qDebug()<<max_action[j].val<<endl;
        if(max_action.size() == 0)
        {
        if(v == -100000) cout<<"human win"<<endl;
        else if(v == 100000) cout<<"machine win"<<endl;
        else cout<<v<<"draw"<<endl;
        return v;
        }
    }
    else
    {
         v = 100000;
    }
    val_return = v;
    return v;
}

//////////////////////////////////////////////
/// \brief check whether win in only one move
/// \param m: a check board
/// \return true/false
///
bool Search::check_depth_zero(node m[6][6])
{
    for(int i = 0; i<board_size; i++)
    for(int j = 0; j<board_size; j++)
    {
        if(m[i][j].type == machine_type)
        {
            for(int f = 0; f<8; f++)
            {
                node t[6][6];
                for(int k = 0; k<board_size; k++)
                for(int l = 0; l<board_size; l++)
                    t[k][l] = m[k][l];
                int bl = black;
                int wh = white;
                if(move_checker(t, i, j, f, bl, wh))
                {
                    int connectnums = 1;
                    if(win(t, machine_type, bl, wh, connectnums))
                    {
                        action ac(100000, f, i, j);
                        max_action.push_back(ac);
                        return true;
                    }
                }
            }
        }
    }
    return false;
}


