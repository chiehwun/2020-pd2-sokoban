#include "player.h"
#include <QDebug>

player::player(QWidget *parent, int Mgrid, int px, int py)
{
    grid = Mgrid;
    lb = new QLabel();
    lb->setGeometry(px, py, grid, grid);
    lb->setParent(parent);
    setplayerIcon(0);
    lb->show();
}

player::~player() {
    delete lb;
}

void player::move(QKeyEvent *e)
{
    if(e->key() == Qt::Key_Up)
        lb->setGeometry(lb->x(), lb->y()-grid, grid, grid);
    else if(e->key() == Qt::Key_Down)
        lb->setGeometry(lb->x(), lb->y()+grid, grid, grid);
    else if(e->key() == Qt::Key_Left)
        lb->setGeometry(lb->x()-grid, lb->y(), grid, grid);
    else if(e->key() == Qt::Key_Right)
        lb->setGeometry(lb->x()+grid, lb->y(), grid, grid);
    qDebug() << "mv: ply.loc = " << lb->x() << ", " << lb->y();
}

void player::setplayerIcon(int i)
{
    QPixmap pix(":/img/player" + QString::number(i));
    lb->setPixmap(pix.scaled(lb->width(), lb->height(), Qt::IgnoreAspectRatio));
    //    lb->setFrameStyle(QFrame::Box);
}

int player::getX()
{
    return lb->x();
}

int player::getY()
{
    return lb->y();
}

