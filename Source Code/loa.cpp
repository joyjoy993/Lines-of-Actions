#include "loa.h"
#include "ui_loa.h"
#include <QShortcut>
#include <QKeySequence>
#include<QCoreApplication>

LOA::LOA(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LOA)
{
    ui->setupUi(this);
}

LOA::~LOA()
{
    delete ui;
}

//////////////////////////////////////////////
/// \brief initial a check board
///
void LOA::initial()
{
    black = 0;
    white = 0;
    cur_player = 0;
    machine_type = 0;
    human_type = 1;
    empty_type = 2;
    time_limit = 11;
    finished = 0;
    curx = -1;
    cury = -1;
    numofsel = 0;
    ui->black->setChecked(true);
    ui->level1->setChecked(true);
    ui->start->setEnabled(true);
    ui->restart->setEnabled(false);
    QShortcut  *shortcut= new QShortcut(QKeySequence(tr("esc")), this);
    connect(shortcut, SIGNAL(activated()), this,SLOT(exit_p()));
    int m_five[6][6] = {{2,1,1,1,2}, {0,2,2,2,0}, {0,2,2,2,0}, {0,2,2,2,0}, {2,1,1,1,2}, {2,2,2,2,2,2}};//initial
    //int m[6][6] = {{2,1,2,2,1}, {2,2,0,0,0}, {0,2,2,1,0}, {2,2,2,2,0}, {2,1,1,1,2}};
    //int m_five[6][6] = {{2,0,0,0,2}, {1,2,2,2,1}, {1,2,2,2,1}, {1,2,2,2,1}, {2,0,0,0,2}, {2,2,2,2,2,2}};//initial

    int m_six[6][6] = {{2,1,1,1,1,2}, {0,2,2,2,2,0}, {0,2,2,2,2,0}, {0,2,2,2,2,0}, {0,2,2,2,2,0}, {2,1,1,1,1,2}};//initial


    for(int i = 0; i<board_size; i++)
        for(int j = 0; j<board_size; j++)
        {
            int tmp;
            if(board_size == 5) tmp = m_five[i][j];
            else tmp = m_six[i][j];
            if(tmp == 1) white++;
            else if(tmp == 0) black++;
            map_loa[i][j] = node(tmp, i, j);
        }
    update_map();
}

//////////////////////////////////////////////
/// \brief move a checker
/// \param x: coordinate x
/// \param y: coordinate y
///
void LOA::move_ch(int x, int y)
{
    if(ui->start->isEnabled()) { setlog("Not started, you cannot move a checker!"); return; }
    if(finished) { setlog("A finished game, you cannot move a checker!"); return; }

    if(numofsel == 0)//no check select, wait for next one
    {
        if(map_loa[x][y].type == human_type)
        {
            numofsel++;
            curx = x;
            cury = y;
            QString str = "Select (" + QString::number(x) + "," + QString::number(y) + ")";
            setlog(str);
        }
        else
          setlog("This is not a human checker!");
    }
    else if (numofsel == 1)//one checker selected before, varify them
    {
        if(!(x==curx && y==cury))
        {
            //compute dir
            int tx, ty;
            int dir = -1;
            for(int k = 0; k<8; k++)
            {
                tx = curx + dir_move[k][0];
                ty = cury + dir_move[k][1];
                while(check_vaild(tx, ty))
                {
                    if(tx == x && ty == y) { dir = k; break; }
                    tx = tx + dir_move[k][0];
                    ty = ty + dir_move[k][1];
                }
                if(dir!=-1) break;
            }
            if(dir!=-1)//valided
            {
                if(move_checker(map_loa, curx, cury, dir, black, white))
                {
                    cleanmap();
                    update_map();

                    if(terminate()) return;

                    curx = -1;
                    cury = -1;
                    numofsel = 0;

                    timer = new QTimer(this);
                    connect(timer,SIGNAL(timeout()),this,SLOT(refreshlcd()));
                    se = new Search(map_loa, black, white, difficulty, time_limit, machine_type, human_type, board_size, level);
                    timer->start(1000);
                    ui->restart->setEnabled(false);
                    ui->reselect->setEnabled(false);
                    se->start();
                    while(!se->finish)
                        QCoreApplication::processEvents();
                    timer->stop();
                    timer->disconnect();
                    delete timer;
                    second = 1;
                    done_search();
                    ui->restart->setEnabled(true);
                    ui->reselect->setEnabled(true);
                }
                else
                    setlog("You can not move to this direction!");
            }
            else
               setlog("This is not a valid direction!");
        }
        else
        {
            setlog("You have chosen this checker!");
        }
    }

}

//////////////////////////////////////////////
/// \brief delete the old widgets, and then update the map
///
void LOA::cleanmap()
{
    for(int i = 0; i<board_size; i++)
        for(int j = 0; j<board_size; j++)
        {
            QWidget * item;
            item = ui->gridLayout->itemAtPosition(i, j)->widget();
            ui->gridLayout->removeWidget(item);
            item->disconnect();
            delete(item);
        }
}

//////////////////////////////////////////////
/// \brief show a new board
///
void LOA::update_map()
{
    Checker *label;
     for(int i = 0; i<board_size; i++)
         for(int j = 0; j<board_size; j++)
         {
             label =  new Checker();
             if(map_loa[j][i].type == 0)//black
                 label->setimg(0);
             else if(map_loa[j][i].type == 1) //white
                 label->setimg(1);
             else label->setimg(2);
             label->x = j;
             label->y = i;
             connect(label, SIGNAL(clicked(int, int)), this, SLOT(move_ch(int, int)));
             ui->gridLayout->addWidget(label, i, j);
         }
     ui->gridLayout->setSpacing(0);
}

//////////////////////////////////////////////
/// \brief show the data of alpha beta search in the widget
///
void LOA::show_data()
{

    ui->data->clear();
    QString str;
    str.sprintf("alpha-beta search return val: %d\nmax depth: %d levels\nexpanded node: %d\nmax eval time: %d times\nmin eval time: %d times\nmax pruning: %d times\nmin pruning: %d times\n", se->val_return, se->max_depth, se->expanded_node, se->max_time, se->min_time, se->max_pruning, se->min_pruning);
    ui->data->setText(str);
    ui->data->repaint();
}

//////////////////////////////////////////////
/// \brief start a game
///
void LOA::on_start_clicked()
{
    if(ui->black->isChecked())
    {
        human_type = 0;
        machine_type = 1;
    }
    else
    {
        human_type = 1;
        machine_type = 0;
    }

    if(ui->level1->isChecked())
    {
        difficulty = 5;
        level = 1;
    }
    else if(ui->level2->isChecked())
    {
        difficulty = 8;
        level = 2;
    }
    else
    {
        difficulty = 8;
        level = 3;
    }

    ui->start->setEnabled(false);
    ui->restart->setEnabled(true);
    ui->change_size->setEnabled(false);

    if(!ui->black->isChecked())
    {
        se = new Search(map_loa, black, white, difficulty, time_limit, machine_type, human_type, board_size, level);
        timer = new QTimer(this);
        connect(timer,SIGNAL(timeout()),this,SLOT(refreshlcd()));
        timer->start(1000);
        ui->restart->setEnabled(false);
        ui->reselect->setEnabled(false);
        se->start();
        while(!se->finish)
            QCoreApplication::processEvents();
        timer->stop();
        timer->disconnect();
        delete timer;
        second = 1;
        done_search();
        ui->restart->setEnabled(true);
        ui->reselect->setEnabled(true);
    }
}

//////////////////////////////////////////////
/// \brief do something after alpha beta search
///
void LOA::done_search()
{
    if(se->max_action.size()==0)
    {
        //cout<<"size=0"<<endl;
        return ;
    }
    move_checker(map_loa, se->max_action[0].x, se->max_action[0].y, se->max_action[0].dir, black, white);

    show_data();
    cleanmap();
    update_map();
    delete se;
    if(terminate()) return;
}

//////////////////////////////////////////////
/// \brief check if the game is end
/// \return true/false
///
bool LOA::terminate()
{
    int temp = 1;
    int winner = 0;
    QString str;
    if(win(map_loa, human_type, black, white, temp) && win(map_loa, machine_type, black, white, temp))
    {
        winner = 1;
        str = "both win";
    }
    else if(win(map_loa, machine_type, black, white, temp))
    {
        winner = 1;
        str = "machine win";
    }
    else if (win(map_loa, human_type, black, white, temp))
    {
        winner = 1;
        str = "human win";
    }

    if(winner != 0)
    {
        finished = 1;
        QMessageBox msgBox;
        msgBox.setText(str);
        msgBox.setInformativeText(tr("Do you want to restart a game?"));
        msgBox.setStandardButtons(QMessageBox::Yes
                                  | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::Yes);
        int ret = msgBox.exec();
        switch (ret) {
        case QMessageBox::Yes:
            setlog("Restart a game!");
            on_restart_clicked();
            break;
        case QMessageBox::No:
            break;
        }
        return 1;
    }
    return 0;
}

//////////////////////////////////////////////
/// \brief restart a game
///
void LOA::on_restart_clicked()
{
    cleanmap();
    initial();
    ui->restart->setEnabled(false);
    ui->start->setEnabled(true);
    ui->change_size->setEnabled(true);
    ui->data->clear();
    ui->lcdNumber->display(0);
    finished = 0;
}

//////////////////////////////////////////////
/// \brief when choose a wrong checker, can reselect the other one
///
void LOA::on_reselect_clicked()
{
    curx = -1;
    cury = -1;
    numofsel = 0;
    setlog("Reselect a checker!");
}

//////////////////////////////////////////////
/// \brief function of refresh the clock
///
void LOA::refreshlcd()
{
    ui->lcdNumber->display(QString::number(second++));
}

//////////////////////////////////////////////
/// \brief setlog
/// \param str
///
void LOA::setlog(QString str)
{
    ui->log->append(str);
}

//////////////////////////////////////////////
/// \brief change board size
///
void LOA::on_change_size_clicked()
{
    cleanmap();
    if(size)
    {
        board_size = 6;
        ui->change_size->setText("change to 5x5 board");
    }
    else
    {
        board_size = 5;
        ui->change_size->setText("change to 6x6 board");
    }
    size = !size;
    initial();
}

void LOA::exit_p()
{
    QCoreApplication::exit();
}
