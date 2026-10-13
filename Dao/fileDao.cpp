#include"fileDao.h"
#include<qsqldatabase.h>
#include<qsqlrecord.h>
#include<qfileinfo.h>

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

//之前误建的旧表叫 file，新表加上 filePath 列后改名 fileInfo，
//这里做一次自动迁移：老表存在就把它改名过来，数据不丢。
//QString fileDao::fileTableName() {
//    QSqlDatabase db = DBManager::instance();
//    if (!db.open()) {
//        qDebug() << "database open failed";
//    }
//    QString name = "fileInfo";
//    QSqlQuery query(db);
//    query.prepare("SELECT name FROM sqlite_master WHERE type='table'");
//    QStringList tables;
//    if (query.exec()) {
//        while (query.next()) {
//            tables.append(query.value(0).toString());
//        }
//    }
//    if (!tables.contains("fileInfo") && tables.contains("file")) {
//        QSqlQuery rename(db);
//        if (rename.exec("ALTER TABLE file RENAME TO fileInfo")) {
//            qDebug() << "file table renamed to fileInfo";
//        }
//        else {
//            qDebug() << "rename file table failed" << rename.lastError();
//        }
//    }
//    db.close();
//    return name;
//}

bool fileDao::initQuery() {
    QSqlDatabase db = DBManager::instance();
    if (!db.open()) {
        qDebug() << "database open failed";
    }
    QString table = "fileInfo";
    //fileTableName 内部重新取过一次连接，这里不需要再 open
    QSqlQuery query(db);
    QString initSql = QString(R"(
    CREATE TABLE IF NOT EXISTS %1(
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    filePath TEXT
    )
    )").arg(table);
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

//注意：这里传进来的是完整路径，不能是 QFileInfo::fileName()
//存成纯文件名后重启就再也定位不到文件了
int fileDao::addFile(QString filePath)
{
    if (filePath.isEmpty()) {
        return -1;
    }
    QSqlDatabase db = DBManager::instance();
    QSqlQuery query(db);
    if (!db.open()) {
        qDebug() << "database open failed";
    }
    //同一个文件不重复入库，否则每次启动都会多出一行
    QSqlQuery findQuery(db);
    findQuery.prepare("SELECT id FROM fileInfo WHERE filePath=:filePath");
    findQuery.bindValue(":filePath", filePath);
    if (findQuery.exec() && findQuery.next()) {
        int existsId = findQuery.value(0).toInt();
        db.close();
        return existsId;
    }

    QString addSql = R"(
    INSERT INTO fileInfo(filePath) VALUES(:filePath);
   )";
    query.prepare(addSql);
    query.bindValue(":filePath", filePath);
    if (query.exec()) {
        //拿自增主键，这才是后续删除该用的id
        int id = query.lastInsertId().toInt();
        qDebug() << "add file success, id =" << id;
        db.close();
        return id;
    }
    qDebug() << "add file failed" << query.lastError();
    db.close();
    return -1;
}

//按主键id删除，调用方传的必须是id，不是model行号
bool fileDao::deleteFile(const int id)
{
    if (id < 0) {
        return false;
    }
    QSqlDatabase db = DBManager::instance();
    QSqlQuery query(db);
    if (!db.open()) {
        qDebug() << "database open failed";
    }
    QString delSql = R"(
    DELETE FROM fileInfo WHERE id=:id
   )";
    query.prepare(delSql);
    query.bindValue(":id", id);
    if (query.exec()) {
        qDebug() << "del file success, id =" << id;
        db.close();
        return true;
    }
    qDebug() << "del file failed" << query.lastError();
    db.close();
    return false;
}

//清掉历史脏数据：以前 addFile(fileName) 存进去的纯文件名，重启后无法定位
void fileDao::removeInvalidRows()
{
    QSqlDatabase db = DBManager::instance();
    QSqlQuery query(db);
    if (!db.open()) {
        qDebug() << "database open failed";
    }
    QSqlQuery selectQuery(db);
    QStringList badIds;
    if (selectQuery.exec("SELECT id, filePath FROM fileInfo")) {
        while (selectQuery.next()) {
            QString path = selectQuery.value(1).toString();
            if (path.isEmpty() || !QFileInfo(path).isAbsolute()) {
                badIds.append(selectQuery.value(0).toString());
            }
        }
    }
    if (badIds.isEmpty()) {
        db.close();
        return;
    }
    QSqlQuery delQuery(db);
    delQuery.prepare("DELETE FROM fileInfo WHERE id=:id");
    int removed = 0;
    for (const QString& id : badIds) {
        delQuery.bindValue(":id", id);
        if (delQuery.exec()) {
            ++removed;
        }
    }
    qDebug() << "removed invalid file records:" << removed;
    db.close();
}

QVector<fileRecord> fileDao::getFileInfos()
{
    QVector<fileRecord> fileInfos;
    QSqlDatabase db = DBManager::instance();
    if (!db.open()) {
        qDebug() << "database open failed";
    }
    QSqlQuery query(db);
    //必须 ORDER BY id，否则行序不保证，model行号和id会对不上
    QString sql = R"(
     SELECT id, filePath FROM fileInfo ORDER BY id
     )";
    query.prepare(sql);
    if (!query.exec()) {
        qDebug() << "dataBase select failed" << query.lastError();
    }
    while (query.next()) {
        fileRecord record;
        record.id = query.value("id").toInt();
        record.path = query.value("filePath").toString();
        fileInfos.append(record);
    }
    db.close();
    return fileInfos;
}