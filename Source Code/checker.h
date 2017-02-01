#ifndef CHECKER_H
#define CHECKER_H
#include <QLabel>
#include <QMouseEvent>
#include <QDebug>
#include <QObject>
#include <QPainter>

class Checker : public QLabel
{
    Q_OBJECT
public:
    explicit Checker(QWidget *parent = 0);
    ~Checker();
    void setimg(int flag);//draw a checker, flag-0 for black checker, flag-1 for white checker
    int x;//coordinate x
    int y;//coordinate y

signals:
    void clicked(int x, int y);//a signal when clicked a checker

protected:
    void enterEvent(QEvent *);//enterEvent
    void leaveEvent(QEvent *);//leaveEvent
    void mouseReleaseEvent(QMouseEvent *e);//mouseReleaseEvent

private:
    QCursor m_preCursor;
};

#endif // CHECKER_H
