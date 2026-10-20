#include"fileDao.h"
#include<qsqldatabase.h>
#include<qsqlrecord.h>
#include<qfileinfo.h>
#include<qdir.h>

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
        qCritical() << "database open failed";
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
        qInfo() << "文件表建立成功";
        db.close();
        return true;
    }
    qWarning() << "文件表创建失败或已存在" << query.lastError();
    db.close();
    return false;
}

//入库前统一路径写法：正斜杠 + 小写盘符。
//Windows 路径大小写不敏感，QFileDialog 给正斜杠、拖拽给反斜杠，
//不统一的话同一个文件会以两种写法各存一份，去重就形同虚设。
QString fileDao::normalizePath(const QString& filePath)
{
    if (filePath.isEmpty()) {
        return QString();
    }
    //存在就取规范全路径，能把 .. 和符号链接都理顺
    QString canonical = QFileInfo(filePath).canonicalFilePath();
    QString result = canonical.isEmpty() ? QFileInfo(filePath).absoluteFilePath() : canonical;
    return QDir::fromNativeSeparators(result).toLower();
}

//注意：这里传进来的是完整路径，不能是 QFileInfo::fileName()
//存成纯文件名后重启就再也定位不到文件了
int fileDao::addFile(QString filePath)
{
    if (filePath.isEmpty()) {
        qInfo() << "数据库添加文件失败,原因：文件路径为空";
        return -1;
    }
    const QString normalized = normalizePath(filePath);

    //每次都要现场取一次连接，不能复用旧引用：
    //DBManager::instance() 每次都会 addDatabase，旧连接会被顶掉
    QSqlDatabase db = DBManager::instance();
    if (!db.open()) {
        qCritical() << "addFile: database open failed" << db.lastError().text();
        return -1;
    }

    //同一个文件不重复入库，否则每次启动都会多出一行
    //这里的 exec 失败必须显式上报：表不存在时它会静默返回false，
    //接着走 INSERT 同样失败，表面上看不出任何问题
    QSqlQuery findQuery(db);
    findQuery.prepare("SELECT id FROM fileInfo WHERE filePath=:filePath");
    findQuery.bindValue(":filePath", normalized);
    if (!findQuery.exec()) {
        qWarning() << "addFile: SELECT failed" << findQuery.lastError().text();
    }
    else if (findQuery.next()) {
        int existsId = findQuery.value(0).toInt();
        qDebug() << "addFile: already exists, id =" << existsId << normalized;
        db.close();
        return -1;
    }

    QSqlQuery insertQuery(db);
    insertQuery.prepare("INSERT INTO fileInfo(filePath) VALUES(:filePath)");
    insertQuery.bindValue(":filePath", normalized);
    if (!insertQuery.exec()) {
        qWarning() << "addFile: INSERT failed" << insertQuery.lastError().text();
        db.close();
        return -1;
    }
    //拿自增主键，这才是后续删除该用的id
    int id = insertQuery.lastInsertId().toInt();
    qDebug() << "addFile: inserted id =" << id << normalized;
    db.close();
    return id;
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
        qCritical() << "database open failed";
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
    qWarning() << "del file failed" << query.lastError();
    db.close();
    return false;
}

//清掉历史脏数据：以前 addFile(fileName) 存进去的纯文件名，重启后无法定位
void fileDao::removeInvalidRows()
{
    QSqlDatabase db = DBManager::instance();
    QSqlQuery query(db);
    if (!db.open()) {
        qCritical() << "database open failed";
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
        qCritical() << "database open failed";
    }
    QSqlQuery query(db);
    //必须 ORDER BY id，否则行序不保证，model行号和id会对不上
    QString sql = R"(
     SELECT id, filePath FROM fileInfo ORDER BY id
     )";
    query.prepare(sql);
    if (!query.exec()) {
        qWarning() << "dataBase select failed" << query.lastError();
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