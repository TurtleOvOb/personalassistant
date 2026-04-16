#include"NoteDao.h"
#include "fileDao.h"
noteDao::noteDao() {
    if (initQuery()) {
        qDebug() << "notetable init success";
    }
    else {
        qDebug() << "notetable init failed";
    }
}

noteDao::~noteDao()
{
}

bool noteDao::initQuery() {
    QSqlDatabase db = DBManager::instance();
    if (!db.open()) {
        qDebug() << "database open failed";
    }
    QSqlQuery query(db);
    QString initSql = R"(
    CREATE TABLE IF NOT EXISTS note(
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    title TEXT,
    des TEXT,
    type INTEGER
    )

    )";
    query.prepare(initSql);
    if (query.exec()) {
        qDebug() << "create note success";
        db.close();
        return true;
    }
    qDebug() << "sql exec failed or table already created" << query.lastError();
    db.close();
    return false;
}

int noteDao::addKard(QString title, QString des, int type)
{
    QSqlDatabase db = DBManager::instance();
    QSqlQuery query(db);
    if (!db.open()) {
        qDebug() << "database open failed";
    }
    QString addSql = R"(
    INSERT INTO note(title,des,type) VALUES(:title,:des,:type);
   )";
    query.prepare(addSql);
    query.bindValue(":title", title);
    query.bindValue(":des", des);
    query.bindValue(":type", type);
    if (query.exec()) {
        qDebug() << "add notekard success";
        int id = query.lastInsertId().toInt();
        db.close();
        return id;
    }
    qDebug() << "add notekard failed";
    db.close();
    return false;
}
bool noteDao::deleteKard(int id) {
    QSqlDatabase db = DBManager::instance();
    QSqlQuery query(db);
    if (!db.open()) {
        qDebug() << "database open failed";
    }
    QString delSql = R"(
    DELETE FROM note WHERE id=:id
   )";
    query.prepare(delSql);
    query.bindValue(":id", id);
    if (query.exec()) {
        qDebug() << "del notekard success";
        db.close();
        return true;
    }
    qDebug() << "del notekard failed";
    db.close();
    return false;
}

bool noteDao::updateKard(int id, QString title, QString des, int type)
{
    QSqlDatabase db = DBManager::instance();
    QSqlQuery query(db);
    if (!db.open()) {
        qDebug() << "database open failed";
    }
    QString sql = R"(
UPDATE note 
SET title=(:title),des=(:des),type=(:type)
WHERE id=(:id)
)";
    query.prepare(sql);
    query.bindValue(":id", id);
    query.bindValue(":title", title);
    query.bindValue(":des", des);
    query.bindValue(":type", type);
    if (query.exec()) {
        qDebug() << "update notekard success";
        db.close();
        return true;
    }
    qDebug() << "update notekard failed";
    db.close();
    return false;
}



QVector<NoteKard*> noteDao::getNoteKards(QWidget* parent)
{
    QVector<NoteKard*>kardList;
    QSqlDatabase db = DBManager::instance();
    if (!db.open()) {
        qDebug() << "database open failed";
    }
    QSqlQuery query(db);
    QString sql = R"(
     SELECT * FROM note
     )";
    query.prepare(sql);
    if (!query.exec()) {
        qDebug() << "dataBase select failed";
    }
    while (query.next()) {
        int id = query.value("id").toInt();
        QString title = query.value("title").toString();
        QString des = query.value("des").toString();
        int type = query.value("type").toInt();
        NoteKard* kard = new NoteKard(parent);
        kard->setId(id);
        kard->setTitle(title);
        kard->setDes(des);
        kard->setType(type,themeManager::instance()->currentTheme());
        kardList.append(kard);
    }
    db.close();
    return kardList;
}
