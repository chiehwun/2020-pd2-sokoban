#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QPixmap>
#include <QKeyEvent>

class player : public QObject
{
    Q_OBJECT

public:
    player(QWidget *parent, int Mgrid, int px, int py);
    ~player();
    void move(QKeyEvent *e);
    void setplayerIcon(int i);
    int getX();
    int getY();

signals:

public slots:

private:
    QLabel *lb;
    int grid;

};

#endif // PLAYER_H
