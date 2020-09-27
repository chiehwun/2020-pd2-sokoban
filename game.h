#ifndef GAME_H
#define GAME_H

#include "map.h"
#include "database.h"
#include <QMainWindow>
#include <QObject>
#include <QLabel>
#include <QKeyEvent>
#include <QTimer>
#include <QTime>
#include <QDateTime>
#include <QMessageBox>

namespace Ui {
class game;
}

class game : public QMainWindow
{
    Q_OBJECT

public:
    explicit game(QString player,
                  int lv,
                  database *dbIn,
                  QWidget *parent = nullptr);
    ~game();
    void setgameLv(int lv);
    void keyPressEvent(QKeyEvent *e);   // Overload function
    void setpassInfo(int s);
    int levelpass;  // -1: gameover, 0: none, 1: pass
    int level;
    QTimer *timer;

private slots:
    void gameOver_slot();
    void on_MenuBtn_clicked();
    void on_ResetBtn_clicked();
    void updateTimer();
    void stopTimer();


private:
    void showEvent(QShowEvent *e);
    database *gmdb;
    QWidget *mainwindow;
    Ui::game *ui;
    map *mp;
    QLabel *timerCnt;
    QLabel *stepCnt;
    QLabel *passInfo;
    QTime *elapseTime;
};

#endif // GAME_H
