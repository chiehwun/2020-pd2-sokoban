#include "database.h"
#include <QDebug>

database::database()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(qApp->applicationDirPath()
                        + QDir::separator()
                        + "gmdb.sqlite");
    if(!db.open())
        qDebug() << db.lastError().text();
    else
        qDebug() << "open database Successfully.";

    QString query1 = "CREATE TABLE rank ("
                     "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                     "LevelType TEXT, "
                     "Level INTEGER, "
                     "PlayerName VARCHAR(20), "
                     "Step INTEGER, "
                     "Time TEXT);";
    QSqlQuery qry;
    if(!qry.exec(query1))
        qDebug() << "error creating table \"rank\" or already exists";
    else
        qDebug() << "Successfully create table \"rank\"";
    QString query2 = "CREATE TABLE name ("
                     "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                     "PlayerName VARCHAR(20), "
                     "NormalRecord INTEGER, "
                     "SpecialRecord INTEGER, "
                     "LastPlay INTEGER, "
                     "LastLogin Boolean);";
    if(!qry.exec(query2))
        qDebug() << "error creating table \"name\" or already exists";
    else
        qDebug() << "Successfully create table \"name\"";

    /********** TEST DATA **********/
//    addRecordDB("Normal", 1, "JoJo", 2, "00:02:0");
//    addRecordDB("Normal", 1, "DIO DAAAA", 1, "00:01:3");
//    addRecordDB("Normal", 1, "ZA-WARUDO", 1, "00:00:1");
//    addNameDB("JoJo");
//    updateRecordDB("JoJo", 1);
//    updateRecordDB("JoJo", 2);
//    updatelastLvDB("JoJo", 7);

//    addNameDB("DIO DAAAA");
//    updateRecordDB("DIO DAAAA", 1);
//    updateRecordDB("DIO DAAAA", 2);
//    updatelastLvDB("DIO DAAAA", 3);
//    updateRecordDB("DIO DAAAA", 6);

//    addNameDB("ZA-WARUDO");
//    updateRecordDB("ZA-WARUDO", 1);
//    updateRecordDB("ZA-WARUDO", 2);
//    updateRecordDB("ZA-WARUDO", 3);
//    updateRecordDB("ZA-WARUDO", 4);
//    updateRecordDB("ZA-WARUDO", 5);
//    updateRecordDB("ZA-WARUDO", 7);
//    updateRecordDB("ZA-WARUDO", 8);
//    updatelastLvDB("ZA-WARUDO", 9);
}

void database::addNameDB(QString name)
{
    QSqlQuery qry;
        qry.exec("SELECT COUNT(*) CNT FROM name WHERE PlayerName == \'" + name + "\';");
        qry.next();
        int count = qry.value(0).toInt();
        qDebug() << name << ": " << count << " results found.";
        qry.clear();
        qry.exec("UPDATE name SET LastLogin = 0;");
        qry.clear();
        if(count == 0) {
            qry.prepare("INSERT INTO name ("
                        "PlayerName, "
                        "NormalRecord, "
                        "SpecialRecord, "
                        "LastPlay, "
                        "LastLogin) "
                        "VALUES (?,?,?,?,?);");
            qry.addBindValue(name);
            qry.addBindValue("0");
            qry.addBindValue("0");
            qry.addBindValue("1");
            qry.addBindValue("1");
            if(!qry.exec())
                qDebug() << "error adding value to name table.";
            else
                qDebug() << "Sucessfully add \"" << name << "\" into Name table.";
        }
        else {
            qry.exec("UPDATE name SET LastLogin = 1 WHERE PlayerName == '"+name+"';");
        }
}

void database::addRecordDB(QString LevelType,
                           int Level,
                           QString PlayerName,
                           int Step,
                           QString Time)
{
    QSqlQuery qry;
        qry.prepare("INSERT INTO rank ("
                    "LevelType,"
                    "Level,"
                    "PlayerName,"
                    "Step,"
                    "Time) "
                    "VALUES (?,?,?,?,?);");
        qry.addBindValue(LevelType);
        qry.addBindValue(Level);
        qry.addBindValue(PlayerName);
        qry.addBindValue(Step);
        qry.addBindValue(Time);
        if(!qry.exec())
            qDebug() << "error adding value to rank table.";
        else
            qDebug() << "Insert into Name table sucessfully.";
}


void database::showNameDB(QListWidget *lw, QLineEdit *le)
{
    le->clear();
    lw->clear();
    QSqlQuery qry;
    QString instr = "SELECT PlayerName FROM name WHERE LastLogin == 1;";
    if(!qry.exec(instr)) {
        qDebug() << instr;
        qDebug() << "error selecting name table.";
    }
    else {
        qry.next();
        le->setText(qry.value(0).toString());
    }
    qry.clear();
    instr = "SELECT PlayerName FROM name;";
    if(!qry.exec(instr)) {
        qDebug() << instr;
        qDebug() << "error selecting name table.";
    }
    else {
        qDebug() << "showNameDB";
        while(qry.next()){
            QString PlayerName  = qry.value(0).toString();
            lw->addItem(PlayerName);
        }
    }
}

void database::showRecordDB(QString LevelType,
                            int Level,
                            QTableWidget *tlw)
{
    QSqlQuery qry;
    QString instr = "SELECT PlayerName, Step, Time FROM rank WHERE "
                    " LevelType == \'" + LevelType + "\'"
                    " AND Level == " + QString::number(Level) +
                    " ORDER BY Step ASC, Time ASC;";
    if(!qry.exec(instr)) {
        qDebug() << instr;
        qDebug() << "error selecting rank table.";
    }
    else {
        qDebug() << "showRecordDB: "+LevelType+" Lv"+QString::number(Level);
        QStringList header;
        header.append("Player"); header.append("Step"); header.append("Time");
        tlw->clear();
        tlw->setRowCount(0);
        tlw->setColumnCount(3);
        tlw->setHorizontalHeaderLabels(header);
        while(qry.next()){
            QString PlayerName  = qry.value(0).toString();
            int step            = qry.value(1).toInt();
            QString Time        = qry.value(2).toString();
            int row = tlw->rowCount();
            tlw->insertRow(row);
            tlw->setItem(row,0,new QTableWidgetItem(PlayerName));
            tlw->setItem(row,1,new QTableWidgetItem(QString::number(step)));
            tlw->setItem(row,2,new QTableWidgetItem(Time));
            qDebug() << PlayerName << QString::number(step) << Time;
        }
        tlw->show();
        tlw->update();
    }
}

void database::updatelastLvDB(QString name, int lv)
{
    if(lv < 1 || lv > 10) {
        qDebug() << "error lv out of range while updating LastPlay";
        return;
    }
    QSqlQuery qry;
    QString instr = "UPDATE name SET LastPlay = "+QString::number(lv)+" WHERE PlayerName = '"+name+"';";
    if(!qry.exec(instr)) {
        qDebug() << instr;
        qDebug() << "error updating LastPlay.";
    }
    else {
        qDebug() << "Update LastPlay successfully.";
    }
}

void database::updateRecordDB(QString name, int lv)
{
    if(lv < 1 || lv > 10) {
        qDebug() << "error lv out of range while updating Record";
        return;
    }
    QSqlQuery qry;
    QString col = lv <= 5? "NormalRecord":"SpecialRecord";
    QString level = QString::number(lv <= 5? lv:lv-5);
    QString instr = "SELECT "+col+" FROM name WHERE PlayerName = '"+name+"';";
    if(!qry.exec(instr)) {
        qDebug() << instr;
        qDebug() << "error selecting name table.";
    }
    else {
        qry.next();
        if((lv <= 5? lv:lv-5) < qry.value(0).toInt()){
            qDebug() << "No need to update Record.";
            return;
        }
    }
    qry.clear();
    QString instr_save = "UPDATE name SET "+col+" = "+level+" WHERE PlayerName = '"+name+"';";
    if(!qry.exec(instr_save)) {
        qDebug() << instr_save;
        qDebug() << "error updating "+col+".";
    }
    else
        qDebug() << "Update "+col+" successfully.";
}

int database::getlastLvDB(QString name)
{
    QSqlQuery qry;
    QString instr = "SELECT LastPlay FROM name WHERE "
                    "PlayerName == '" + name + "';";
    if(!qry.exec(instr)) {
        qDebug() << instr;
        qDebug() << "error selecting name table.";
        return -1;
    }
    else {
        qry.next();
        int LastPlay = qry.value(0).toInt();
        qDebug() << "LastPlay = " << QString::number(LastPlay);
        return LastPlay;
    }
}

int database::getRecordDB(QString name, QString Type)
{
    QSqlQuery qry;
    QString col = Type + "Record";
    QString instr = "SELECT "+col+" FROM name WHERE "
                    "PlayerName == '" + name + "';";
    if(!qry.exec(instr)) {
        qDebug() << instr;
        qDebug() << "error selecting name table.";
        return -1;
    }
    else {
        qry.next();
        int Record = qry.value(0).toInt();
        qDebug() << col+"=" << QString::number(Record);
        return Record;
    }
}

