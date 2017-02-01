#include "checker.h"

Checker::Checker(QWidget *parent) : QLabel(parent)
{
    setStyleSheet("QLabel{border:1px solid rgb(0, 0, 0);}");
}

Checker::~Checker()
{

}
//////////////////////////////////////////////
/// \brief draw a checker
/// \param flag: 0 for black, 1 for white
///
void Checker::setimg(int flag)
{
    QPixmap pixmap(100,100);
    pixmap.fill(Qt::gray);
    QPainter painter(&pixmap);
        if(flag == 0)
            painter.setBrush(Qt::black);
        else if(flag == 1)
            painter.setBrush(Qt::white);
        if(flag!=2) painter.drawEllipse(20,30,40,40);
    setPixmap(pixmap);
}

//////////////////////////////////////////////
/// \brief Checker::enterEvent
///
void Checker::enterEvent(QEvent *)
{
    m_preCursor = cursor();
    setCursor(Qt::PointingHandCursor);
}

//////////////////////////////////////////////
/// \brief Checker::leaveEvent
///
void Checker::leaveEvent(QEvent *)
{
    setCursor(m_preCursor);
}

//////////////////////////////////////////////
/// \brief Checker::mouseReleaseEvent
///
void Checker::mouseReleaseEvent(QMouseEvent * /*e*/)
{
    emit clicked(x, y);
}
