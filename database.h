#ifndef DATABASE_H
#define DATABASE_H

#include <QtSql>
#include <QListWidget>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QLineEdit>

class database
{
public:
    database();
    void addNameDB(QString name);
    void addRecordDB(QString LevelType,
                     int Level,
                     QString PlayerName,
                     int Step,
                     QString Time);
    void showNameDB(QListWidget *lw, QLineEdit *le);
    void showRecordDB(QString LevelType,
                      int Level,
                      QTableWidget *tlw);
    void updatelastLvDB(QString name, int lv);
    void updateRecordDB(QString name, int lv);
    int getlastLvDB(QString name);
    int getRecordDB(QString name, QString Type);
private:
    QSqlDatabase db;
};

#endif // DATABASE_H
