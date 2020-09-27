#include "selectlevel.h"
#include "ui_selectlevel.h"
#include <QDebug>

selectlevel::selectlevel(game *gmIn,
                         QString name,
                         database *gmdbIn,
                         QWidget *parent) :
    QMainWindow(parent),
    gm(gmIn),
    ui(new Ui::selectlevel),
    mainwindow(parent)
{
    ui->setupUi(this);
    QVector<QPushButton*> buttons = {ui->N1_btn,
                                     ui->N2_btn,
                                     ui->N3_btn,
                                     ui->N4_btn,
                                     ui->N5_btn,
                                     ui->S1_btn,
                                     ui->S2_btn,
                                     ui->S3_btn,
                                     ui->S4_btn,
                                     ui->S5_btn};
    QSignalMapper *BtnMapper = new QSignalMapper(this);
    for (int i=0; i < buttons.size(); i++) {
        connect(buttons.at(i), SIGNAL(clicked()), BtnMapper, SLOT(map()));
        BtnMapper->setMapping(buttons.at(i), i+1);
    }
    connect(BtnMapper, SIGNAL(mapped(int)), this, SLOT(Doclicked(int)));
    int NRecord = gmdbIn->getRecordDB(name, "Normal");
    int SRecord = gmdbIn->getRecordDB(name, "Special");
    for(auto btn=buttons.begin()+NRecord+1; btn < buttons.begin()+5; btn++)
        (*btn)->setDisabled(true);
    for(auto btn=buttons.begin()+SRecord+6; btn < buttons.end(); btn++)
        (*btn)->setDisabled(true);
}

selectlevel::~selectlevel()
{
    delete ui;
    mainwindow->show();
    this->close();
}

void selectlevel::Doclicked(int buttonID)
{
    qDebug() << "Level Code: " << buttonID;
    gm->setgameLv(buttonID);
    gm->show();
    hide();
}

void selectlevel::on_Menu_btn_clicked()
{
//    mainwindow->show();
    delete this;
}
