#ifndef SELECTLEVEL_H
#define SELECTLEVEL_H

#include <QMainWindow>
#include "game.h"
#include "database.h"
#include <QSignalMapper>


namespace Ui {
class selectlevel;
}

class selectlevel : public QMainWindow
{
    Q_OBJECT

public:
    explicit selectlevel(game *gmIn,
                         QString name,
                         database *gmdbIn,
                         QWidget *parent = nullptr);
    ~selectlevel();
    game *gm;

private slots:
    void Doclicked(int);

    void on_Menu_btn_clicked();

private:
    Ui::selectlevel *ui;
    QWidget *mainwindow;
};

#endif // SELECTLEVEL_H
