#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("SOKOBAN Menu");
    // game window object
    connect(ui->Start_Btn, SIGNAL(clicked()), this, SLOT(start()));
    connect(ui->Chlv_Btn, SIGNAL(clicked()), this, SLOT(chgLv()));
    connect(ui->Quit_Btn, SIGNAL(clicked()), this, SLOT(quit()));
    gmdb = new database;
    gmdb->showNameDB(ui->listWidget, ui->playerName_lineEdit);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showEvent(QShowEvent *e)
{
    gmdb->showNameDB(ui->listWidget, ui->playerName_lineEdit);
}

void MainWindow::start()
{
    QString name = ui->playerName_lineEdit->text();
    if(name == "") {
        ui->statusBar->showMessage("Player Name is empty", 1800);
        return;
    }
    gmdb->addNameDB(name);
    gm = new game(name, gmdb->getlastLvDB(name), gmdb, this);
    gm->timer->stop();
    hide(); // hide mainwindow
    qDebug() << "start";
    gm->show();
}

void MainWindow::chgLv()
{
    QString name = ui->playerName_lineEdit->text();
    if(name == "") {
        ui->statusBar->showMessage("Player Name is empty", 1800);
        return;
    }
    gmdb->addNameDB(name);
    gm = new game(ui->playerName_lineEdit->text(), 0, gmdb, this);
    gm->timer->stop();
    selLv = new selectlevel(gm, name, gmdb, this);
    selLv->show();
    selLv->setWindowModality(Qt::WindowModal);
    hide();
}

void MainWindow::quit() {
    QMessageBox::StandardButton reply;
    QString title = "Warning";
    QString text = "Are you sure you want to Quit?";
    reply = QMessageBox::question(this, title, text,
                            QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes)
       QApplication::quit();
}

void MainWindow::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    ui->playerName_lineEdit->setText(item->text());
}

void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{
    ui->statusBar->showMessage("Double Click to fill \"" + item->text() + "\" automatically", 1500);
}
