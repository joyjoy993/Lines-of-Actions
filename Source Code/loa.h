#ifndef LOA_H
#define LOA_H
#include <board.h>
#include <search.h>

namespace Ui {
class LOA;
}

class LOA :public QMainWindow, public Board
{
    Q_OBJECT

public:
    explicit LOA(QWidget *parent = 0);
    void initial();//initial a check board
    void update_map();//show a new board
    void show_data();//show the data of alpha beta search in the widget
    bool terminate();//check if the game is end
    void cleanmap();//delete the old widgets, and then update the map
    void done_search();//do something after alpha beta search
    ~LOA();

    int numofsel = 0;//# of checkers selected, max is 2
    int curx = -1;//coordinate x of current selected checker
    int cury = -1;//coordinate y of current selected checker
    bool cur_player = 0;//0-black, 1-white
    bool finished = 1;//if a finished game, can not move the checkers any more
    int second = 1;//show moving time
    bool size = true;//true-five, false-six
    int level = 1;//level

    Search *se;//object of alpha beta search
    QTimer *timer;//counting time

public slots:
    void move_ch(int x, int y);//function of move a checker
    void refreshlcd();//function of refresh the clock
    void setlog(QString str);//set log
    void exit_p();//exit program

private slots:
    void on_start_clicked();//button of starting a game
    void on_restart_clicked();//button of restarting a game
    void on_reselect_clicked();//when choose a wrong checker, can reselect the other one
    void on_change_size_clicked();//change board size

private:
    Ui::LOA *ui;
};

#endif // LOA_H
