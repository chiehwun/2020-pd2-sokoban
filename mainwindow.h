#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "game.h"
#include "selectlevel.h"
#include "database.h"
#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <QString>
#include <QListWidget>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    game *gm;
    selectlevel *selLv;

private:
    Ui::MainWindow *ui;
    database *gmdb;
    void showEvent(QShowEvent *e);

private slots:
    void start();
    void chgLv();
    void quit();
    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);
    void on_listWidget_itemClicked(QListWidgetItem *item);
};

#endif // MAINWINDOW_H
