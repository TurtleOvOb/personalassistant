#include"fileDao.h"

fileDao::fileDao()
{
    if (initQuery()) {
        qDebug() << "filetable init success";
    }
    else {
        qDebug() << "filetable init failed";
    }
}

fileDao::~fileDao()
{
}

bool fileDao::initQuery() {
    QSqlDatabase db = DBManager::instance();
    if (!db.open()) {
        qDebug() << "database open failed";
    }
    QSqlQuery query(db);
    QString initSql = R"(
    CREATE TABLE IF NOT EXISTS file(
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    filePath TEXT
    )

    )";
    query.prepare(initSql);
    if (query.exec()) {
        qDebug() << "create file success";
        db.close();
        return true;
    }
    qDebug() << "sql exec failed or table already created" << query.lastError();
    db.close();
    return false;
}


int fileDao::addFile(QString filePath)
{
    QSqlDatabase db = DBManager::instance();
    QSqlQuery query(db);
    if (!db.open()) {
        qDebug() << "database open failed";
    }
    QString addSql = R"(
    INSERT INTO file(filePath) VALUES(:filePath);
   )";
    query.prepare(addSql);
    query.bindValue(":filePath", filePath);
    if (query.exec()) {
        qDebug() << "add file success";
        db.close();
        return true;
    }
    qDebug() << "add file failed";
    db.close();
    return false;
}

bool fileDao::deleteFile(int row)
{
    QSqlDatabase db = DBManager::instance();
    QSqlQuery query(db);
    if (!db.open()) {
        qDebug() << "database open failed";
    }
    QString delSql = R"(
    DELETE FROM file WHERE id=:row
   )";
    query.prepare(delSql);
    query.bindValue(":id", row);
    if (query.exec()) {
        qDebug() << "del todokard success";
        db.close();
        return true;
    }
    qDebug() << "del todokard failed";
    db.close();
    return false;
}


QStringList fileDao::getFileInfos()
{
    QStringList fileInfos;
    QSqlDatabase db = DBManager::instance();
    if (!db.open()) {
        qDebug() << "database open failed";
    }
    QSqlQuery query(db);
    QString sql = R"(
     SELECT * FROM file
     )";
    query.prepare(sql);
    if (!query.exec()) {
        qDebug() << "dataBase select failed";
    }
    while (query.next()) {

        QString filePath = query.value("filePath").toString();
        fileInfos.append(filePath);
    }
    db.close();
    return fileInfos;
}
