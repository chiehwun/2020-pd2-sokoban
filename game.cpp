#include "game.h"
#include "ui_game.h"
#include "game.h"
#include <QDebug>

game::game(QString player,
           int lv,
           database *dbIn,
           QWidget *parent) :
    QMainWindow(parent),
    levelpass(0),
    level(lv),
    gmdb(dbIn),
    mainwindow(parent),
    ui(new Ui::game),
    mp(nullptr)
{
    ui->setupUi(this);
    setWindowTitle("SOKOBAN Gaming");
    // UI setting
    stepCnt = ui->stepCnt_lab;
    passInfo = ui->pass_lab;
    timerCnt = ui->timerLab;
    ui->player_lab->setText(player);

    // Create Timer object
    elapseTime = new QTime(0,0,0);
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateTimer()));
    setgameLv(level);
}

game::~game()
{
    delete ui;
}

void game::setgameLv(int lv)
{
    level = lv;
    // UI setting
    gmdb->updatelastLvDB(ui->player_lab->text(), level);
    gmdb->showRecordDB( lv <= 5? "Normal":"Special",
                        lv <= 5? lv:lv-5,
                        ui->tableWidget);
    if(level <= 5) {
        ui->type_lab->setText("Normal");
        ui->level_lab->setText("Level "+QString::number(level));
    }
    else {
        ui->type_lab->setText("Special");
        ui->level_lab->setText("Level "+QString::number(level-5));
    }
    if(mp == nullptr)
        qDebug() << "nullptr";
    else {
        qDebug() << "NOT nullptr";
        delete mp;
        qDebug() << "map delete";
    }
    mp = new map(this, level);
    qDebug() << "map reset";
    levelpass = 0;
    stepCnt->setNum(0);
    setpassInfo(0);
    // Timer reset and show
    elapseTime->setHMS(0,0,0);
    timerCnt->setText(elapseTime->toString("mm:ss"));
    timer->start(100);
    // Update gameOver() SIGNAL
    connect(mp, SIGNAL(gameOver()), this, SLOT(gameOver_slot()));
}

void game::keyPressEvent(QKeyEvent *e)
{
    if(levelpass == 0 && (e->key() == Qt::Key_Up ||
                      e->key() == Qt::Key_Down ||
                      e->key() == Qt::Key_Left ||
                      e->key() == Qt::Key_Right)) {
        mp->gaming(e);
        if(levelpass == -1) // gameover
            return;
        stepCnt->setNum(mp->stepNum);
        levelpass = mp->checkPass();
        if(levelpass == 1) {
            stopTimer();
            QString name = ui->player_lab->text();
            gmdb->addRecordDB(level <= 5? "Normal":"Special",
                              level <= 5? level:level-5,
                              name,
                              mp->stepNum,
                              timerCnt->text());
            gmdb->showRecordDB(level <= 5? "Normal":"Special",
                               level <= 5? level:level-5,
                               ui->tableWidget);
            gmdb->updateRecordDB(name, level);  // Complete this level
            setpassInfo(1);
        }
    }
    else if(levelpass != 1 && e->key() == Qt::Key_Space) {
        on_ResetBtn_clicked();
    }
    else if(levelpass == 1 && (e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter)) {
        level++;
        on_ResetBtn_clicked();
    }
    else if(e->key() == Qt::Key_M)
        on_MenuBtn_clicked();
}

void game::showEvent(QShowEvent *e)
{
    if(!levelpass)
        timer->start();
}

void game::setpassInfo(int s)
{
    if(s == 0) {
        passInfo->setText("");
        ui->pass_lab_2->setText("");
    }
    else if(s == 1) {
        passInfo->setText("Level Pass!!!");
        ui->pass_lab_2->setText("<Press Enter Key>");
    }
    else if(s == -1) {
        passInfo->setText("GAMEOVER!!!");
        ui->pass_lab_2->setText("<Press Space Key>");
    }
    passInfo->show();
    // prevent messy UI
}

void game::gameOver_slot()
{
    stopTimer();
    setpassInfo(-1);
    levelpass = -1;
}

void game::on_MenuBtn_clicked()
{
    timer->stop();
    hide();
    mainwindow->show();
}

void game::on_ResetBtn_clicked()
{
    setgameLv(level);
}

void game::updateTimer()
{
    *elapseTime = elapseTime->addMSecs(100);
    QString t = elapseTime->toString("mm:ss");
    if(elapseTime->msec() >= 500) {
        t[2] = ' ';
    }
    timerCnt->setText(t);
}

void game::stopTimer()
{
    timer->stop();
    QString t = elapseTime->toString("mm:ss:z");
    timerCnt->setText(t);
}
