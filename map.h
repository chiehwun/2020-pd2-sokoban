#ifndef MAP_H
#define MAP_H

#include "player.h"
#include <QVector>
#include <QObject>
#include <QWidget>
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QLabel>
#include <QtMath>
#include <QKeyEvent>
#include <QPixmap>

class map : public QWidget
{
    Q_OBJECT
public:
    map(QWidget *parent, int level);
    ~map();
    void gaming(QKeyEvent *e);
    bool checkPass();
    void gameover();
    int stepNum;

signals:
    void gameOver();

private:
    /********* NORMAL RULES OBJECTS *********/
    player *ply;                        //P
    QVector<QVector<QLabel*>> ground;   //0
    QVector<QVector<QLabel*>> wall;     //1
    QVector<QVector<QLabel*>> dotbox;   //2
    QVector<QVector<QLabel*>> box;      //3
    /********* SPECIAL RULES OBJECTS *********/
    QVector<QVector<QLabel*>> dotRbox;          //4
    QVector<QVector<QLabel*>> Rbox;             //5
    QVector<QVector<QLabel*>> hole; //mapcode(X,6,7)->linewidth[0,1,2]
    QVector<QVector<QLabel*>> Hbox;             //9
    /********* GENERAL FUNC. *********/
    void createObject(QWidget *parent, int gx, int gy, QString file, QVector<QVector<QLabel*>> &obj, int status=0);
    void setObjPic(QLabel* obj, QString file);
    bool isdotBox(QVector<QLabel*>::iterator b, const QVector<QVector<QLabel*>> &DOTBOX);
    int coord2area(int x, int y);
    int grid2area(int gx, int gy);
    void dxdy(QKeyEvent *e, int &dx, int &dy, int &area);
    /********* NORMAL OBJECTS BEHAVIORS *********/
    bool collWall(int dx, int dy);
    bool collBox(QKeyEvent *e, int dx, int dy);
    void moveBox(QKeyEvent *e, QVector<QLabel*>::iterator b);
    /********* SPECIAL OBJECTS BEHAVIORS *********/
    bool collhole(int dx, int dy);
    bool collRBox(QKeyEvent *e, int dx, int dy);
    void moveRBox(QKeyEvent *e, QVector<QLabel*>::iterator b);
    bool collHbox(QKeyEvent *e, int dx, int dy);
    void moveHBox(QKeyEvent *e, QVector<QLabel*>::iterator b);
    /********* MAP PARAMETERS *********/
    const int grid;
    int mapX;
    int mapY;
    int orgX;
    int orgY;
    /********* WIN PARAMETERS *********/
    int dotboxNum;
    int dotRboxNum;

};

#endif // MAP_H
