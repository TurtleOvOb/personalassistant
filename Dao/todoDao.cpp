#include "todoDao.h"
#include "NoteDao.h"

todoDao::todoDao()
{
    if (initQuery()) {
        qDebug() << "table init success";
    }
    else {
        qDebug() << "table init failed";
    }
    
}

todoDao::~todoDao()
{
}

bool todoDao::initQuery()
{
	QSqlDatabase db = DBManager::instance();
    if (!db.open()) {
        qDebug() << "database open failed";
    }
	QSqlQuery query(db);
	QString initSql = R"(
    CREATE TABLE IF NOT EXISTS todo(
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    title TEXT,
    des TEXT,
    deadLine DATE,
    priority TEXT
    )

    )";
    query.prepare(initSql);
    if (query.exec()) {
        qDebug() << "create todo success";
        db.close();
        return true;
    }
    qDebug() << "sql exec failed or table already created" << query.lastError();
    db.close();
	return false;
}

int todoDao::addKard(QString title, QString des, QDate deadLine, QString priority)
{

    QSqlDatabase db = DBManager::instance();
    QSqlQuery query(db);
    if (!db.open()) {
        qDebug() << "database open failed";
    }
    QString addSql = R"(
    INSERT INTO todo(title,des,deadLine,priority) VALUES(:title,:des,:deadLine,:priority);
   )";
    query.prepare(addSql);
    query.bindValue(":title", title);
    query.bindValue(":des", des);
    query.bindValue(":deadLine", deadLine);
    query.bindValue(":priority", priority);
    if (query.exec()) {
        qDebug() << "add todokard success";
        int id = query.lastInsertId().toInt();
        db.close();
        return id;
    }
    qDebug() << "add todokard failed";
    db.close();
    return false;
}

bool todoDao::deleteKard(int id)
{
    QSqlDatabase db = DBManager::instance();
    QSqlQuery query(db);
    if (!db.open()) {
        qDebug() << "database open failed";
    }
    QString delSql = R"(
    DELETE FROM todo WHERE id=:id
   )";
    query.prepare(delSql);
    query.bindValue(":id", id);
    if (query.exec()) {
        qDebug() << "del todokard success";
        db.close();
        return true;
    }
    qDebug() << "del todokard failed";
    db.close();
    return false;
}

bool todoDao::updateKard(int id, QString title, QString des, QDate deadLine, QString priority)
{
    QSqlDatabase db = DBManager::instance();
    QSqlQuery query(db);
    if (!db.open()) {
        qDebug() << "database open failed";
    }
    QString sql = R"(
UPDATE todo 
SET title=(:title),des=(:des),deadLine=(:deadLine),priority=(:priority) 
WHERE id=(:id)
)";
    query.prepare(sql);
    query.bindValue(":id", id);
    query.bindValue(":title", title);
    query.bindValue(":des", des);
    query.bindValue(":deadLine", deadLine);
    query.bindValue(":priority", priority);
    if (query.exec()) {
        qDebug() << "update todokard success";
        db.close();
        return true;
    }
    qDebug() << "update todokard failed";
    db.close();
    return false;
}

QVector<toDoKard*> todoDao::getToDoKards(QWidget* parent)
{
    QVector<toDoKard*>kardList;
    QSqlDatabase db = DBManager::instance();
    if (!db.open()) {
        qDebug() << "database open failed";
    }
    QSqlQuery query(db);
    QString sql = R"(
     SELECT * FROM todo
     )"; 
    query.prepare(sql);
    if (!query.exec()) {
        qDebug() << "dataBase select failed";
    }
    while (query.next()) {
        int id = query.value("id").toInt();
        QString title = query.value("title").toString();
        QString des = query.value("des").toString();
        QDate date = query.value("deadLine").toDate();
        QString prior = query.value("priority").toString();
        toDoKard* kard = new toDoKard(parent);
        kard->setId(id);
        kard->setTitle(title);
        kard->setDescription(des);
        kard->setDate(date);
        kard->setPriority(prior);
        kardList.append(kard);
    }
    db.close();
    return kardList;
}
